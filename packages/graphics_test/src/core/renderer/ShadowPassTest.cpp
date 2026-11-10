#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/core/renderer/ShadowPass.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/DirectionalLight.h>
#include <graphics/lights/PointLight.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/objects/Mesh.h>
#include <graphics/objects/InstancedMesh.h>
#include <graphics/textures/Texture2D.h>
#include <common/PrintHelpers.h>
#include <cmath>
#include <limits>

/**
 * Tests directional and spotlight shadow resources and projection state without pixel readback.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_directionalAndSpotShadows)
{
    auto scene = Scene::create();
    auto directional = DirectionalLight::create(Vector3::MINUS_ZHAT);
    auto spot = SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT);
    for (const LightPtr& light : {LightPtr(directional), LightPtr(spot)})
    {
        light->shadow.width = 32;
        light->shadow.height = 64;
        scene->add(light);
    }
    auto material = MeshMaterial::create();
    scene->add(Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), material));
    auto instances = InstancedMesh::create(BoxGeometry::create(1.f, 1.f, 1.f), material);
    instances->addInstance({Vector3(1.f, 0.f, -2.f)});
    scene->add(instances);

    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.lights.size(), 2u);
    BOOST_REQUIRE_EQUAL(state.items.size(), 2u);
    BOOST_TEST(state.items[0].geometryBuffer != nullptr);
    BOOST_TEST(state.items[1].instanceBuffer != nullptr);

    ShadowPass pass;
    pass.render(state, 2);

    for (unsigned int i = 0; i < state.lights.size(); ++i)
    {
        const auto& light = state.lights[i];
        BOOST_TEST(light.shadowIndex == static_cast<int>(i));
        BOOST_REQUIRE(light.shadowMap != nullptr);
        BOOST_TEST(light.shadowMap->width() == 32u);
        BOOST_TEST(light.shadowMap->height() == 64u);
        BOOST_TEST(light.shadowMap->format() == PixelFormat::DEPTH32F);
    }
    BOOST_TEST(state.lights[0].shadowMap != state.lights[1].shadowMap);
    const auto& config = directional->shadow;
    auto expectedDirectional = Matrix4::fromOrthographic(config.directional.width,
        config.directional.height, config.near, config.far).multiply(
            Matrix4::fromTranslation(Vector3(0.f, 0.f, -(config.near * 0.5f + config.far * 0.5f))));
    auto expectedSpot = Matrix4::fromPerspective(spot->outerAngle * 2.f * std::acos(-1.f) / 180.f,
        1.f, spot->shadow.near, spot->shadow.far);
    BOOST_TEST(state.lights[0].shadowTransform == expectedDirectional);
    BOOST_TEST(state.lights[1].shadowTransform == expectedSpot);
}

/**
 * Tests shadow budget and direct-light selection through prepared light state.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_shadowCapacity)
{
    auto scene = Scene::create();
    scene->add(AmbientLight::create());
    auto disabled = DirectionalLight::create(Vector3::MINUS_ZHAT);
    disabled->shadow.enabled = false;
    scene->add(disabled);
    scene->add(PointLight::create()); // Unsupported shadows still consume a direct-light slot.
    auto invalid = SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT);
    invalid->shadow.near = 0.f;
    scene->add(invalid);
    for (unsigned int i = 3; i < MAX_LIGHTS + 1; ++i)
    {
        auto light = DirectionalLight::create(Vector3::MINUS_ZHAT);
        light->shadow.width = light->shadow.height = 32;
        scene->add(light);
    }

    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.lights.size(), MAX_LIGHTS + 2u);
    ShadowPass pass;
    pass.render(state, std::numeric_limits<unsigned int>::max());

    // Ambient, disabled, point, and invalid lights do not consume shadow slots.
    // The first twelve eligible direct lights fill the budget in traversal order.
    for (unsigned int i = 0; i < state.lights.size(); ++i)
    {
        const auto& light = state.lights[i];
        BOOST_REQUIRE(light.light != nullptr); // Unshadowed lights remain available for lighting.
        if (i >= 4 && i < 4 + MAX_SHADOW_LIGHTS)
        {
            BOOST_TEST(light.shadowIndex == static_cast<int>(i - 4));
            BOOST_REQUIRE(light.shadowMap != nullptr);
        }
        else
        {
            BOOST_TEST(light.shadowIndex == -1);
            BOOST_TEST(light.shadowMap == nullptr);
            BOOST_TEST(light.shadowTransform == Matrix4::IDENTITY);
        }
    }
    // A light within the direct-light limit falls back when the shadow budget is full;
    // the seventeenth direct light is also excluded from shadow selection.
    BOOST_TEST(state.lights[MAX_LIGHTS].light->lightType == LightType::DIRECTIONAL);
    BOOST_TEST(state.lights[MAX_LIGHTS + 1].light->lightType == LightType::DIRECTIONAL);
}

/**
 * A zero budget leaves an eligible light available without allocating a shadow map.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_zeroBudget)
{
    auto light = DirectionalLight::create(Vector3::MINUS_ZHAT);
    DrawState state;
    state.lights.push_back({light.get(), Vector3::ZERO, Vector3::MINUS_ZHAT});
    ShadowPass pass;
    pass.render(state, 0);
    BOOST_TEST(state.lights[0].light == light.get());
    BOOST_TEST(state.lights[0].shadowMap == nullptr);
    BOOST_TEST(state.lights[0].shadowIndex == -1);
    BOOST_TEST(state.lights[0].shadowTransform == Matrix4::IDENTITY);
}

/**
 * A requested budget of one shadows only the first eligible light.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_requestedBudget)
{
    auto first = DirectionalLight::create(Vector3::MINUS_ZHAT);
    auto second = SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT);
    first->shadow.width = first->shadow.height = 32;
    second->shadow.width = second->shadow.height = 32;
    DrawState state;
    state.lights.push_back({first.get(), Vector3::ZERO, Vector3::MINUS_ZHAT});
    state.lights.push_back({second.get(), Vector3::ZERO, Vector3::MINUS_ZHAT});
    ShadowPass pass;
    pass.render(state, 1);
    BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
    BOOST_TEST(state.lights[0].shadowIndex == 0);
    BOOST_TEST(state.lights[1].light == second.get());
    BOOST_TEST(state.lights[1].shadowMap == nullptr);
    BOOST_TEST(state.lights[1].shadowIndex == -1);
}

/**
 * The direct-light limit excludes later lights even when shadow slots are available.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_directLightLimit)
{
    auto scene = Scene::create();
    scene->add(AmbientLight::create());
    for (unsigned int i = 0; i < MAX_LIGHTS - 1; ++i)
        scene->add(PointLight::create());
    for (unsigned int i = 0; i < 2; ++i)
    {
        auto light = DirectionalLight::create(Vector3::MINUS_ZHAT);
        light->shadow.width = light->shadow.height = 32;
        scene->add(light);
    }
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.lights.size(), MAX_LIGHTS + 2u);
    ShadowPass pass;
    pass.render(state, 2);
    BOOST_TEST(state.lights[MAX_LIGHTS].shadowIndex == 0);
    BOOST_REQUIRE(state.lights[MAX_LIGHTS].shadowMap != nullptr);
    BOOST_TEST(state.lights[MAX_LIGHTS + 1].shadowIndex == -1);
    BOOST_TEST(state.lights[MAX_LIGHTS + 1].shadowMap == nullptr);
}

/**
 * Invalid projections are skipped without consuming the valid light's shadow slot.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_invalidProjections)
{
    std::vector<LightPtr> lights;
    auto directional = [&]() {
        auto light = DirectionalLight::create(Vector3::MINUS_ZHAT);
        light->shadow.width = light->shadow.height = 32;
        lights.push_back(light);
        return light;
    };
    directional()->shadow.width = 0;
    directional()->shadow.height = -1;
    directional()->shadow.width = std::numeric_limits<int>::max();
    directional()->shadow.near = 0.f;
    auto reversed = directional();
    reversed->shadow.far = reversed->shadow.near;
    directional()->shadow.near = std::numeric_limits<float>::quiet_NaN();
    directional()->shadow.far = std::numeric_limits<float>::infinity();
    directional()->shadow.directional.width = -1.f;
    directional()->shadow.directional.height = 0.f;
    directional()->shadow.directional.center.x = std::numeric_limits<float>::infinity();
    for (float angle : {0.f, 90.f, std::numeric_limits<float>::quiet_NaN()})
    {
        auto spot = SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT);
        spot->outerAngle = angle;
        lights.push_back(spot);
    }
    directional(); // Invalid world-space direction below.
    auto valid = directional();
    DrawState state;
    state.lights.push_back({}); // Null entries must also be ignored.
    for (const auto& light : lights)
        state.lights.push_back({light.get(), Vector3::ZERO, Vector3::MINUS_ZHAT});
    state.lights[state.lights.size() - 2].direction = Vector3::ZERO;

    ShadowPass pass;
    pass.render(state, 1);
    for (unsigned int i = 0; i + 1 < state.lights.size(); ++i)
    {
        BOOST_TEST_CONTEXT("light=" << i)
        {
            BOOST_TEST(state.lights[i].shadowMap == nullptr);
            BOOST_TEST(state.lights[i].shadowIndex == -1);
            BOOST_TEST(state.lights[i].shadowTransform == Matrix4::IDENTITY);
        }
    }
    BOOST_TEST(state.lights.back().light == valid.get());
    BOOST_REQUIRE(state.lights.back().shadowMap != nullptr);
    BOOST_TEST(state.lights.back().shadowIndex == 0);
}

