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
#include <graphics/textures/TextureCube.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/renderer/ScopedRenderState.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/ShadowDepthShader.h>
#include <common/PrintHelpers.h>
#include <glad/glad.h>
#include <cmath>
#include <limits>
#include <set>
#include <sstream>

struct ShadowDiagnostics
{
    std::ostringstream output;
    std::streambuf* previous = std::cerr.rdbuf(output.rdbuf());
    ~ShadowDiagnostics() { std::cerr.rdbuf(previous); }
};

struct IncompleteFramebuffer
{
    inline static PFNGLCHECKFRAMEBUFFERSTATUSPROC previous;

    IncompleteFramebuffer()
    {
        previous = glad_glCheckFramebufferStatus;
        glad_glCheckFramebufferStatus = failOnce;
    }

    ~IncompleteFramebuffer() { glad_glCheckFramebufferStatus = previous; }
    
    static GLenum APIENTRY failOnce(GLenum)
    {
        glad_glCheckFramebufferStatus = previous;
        return GL_FRAMEBUFFER_UNSUPPORTED;
    }
};

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
        const auto& sampling = light.shadowMap->config().sampling;
        BOOST_CHECK(sampling.sWrap == TextureWrap::CLAMP_TO_BORDER);
        BOOST_CHECK(sampling.tWrap == TextureWrap::CLAMP_TO_BORDER);
        for (float channel : sampling.borderColor) BOOST_TEST(channel == 1.f);
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
    // Filling either partition must leave the other partition available.
    for (bool pointsFirst : {false, true})
    {
        auto point = PointLight::create();
        auto directional = DirectionalLight::create(Vector3::MINUS_ZHAT);
        point->shadow.width = point->shadow.height = 32;
        directional->shadow.width = directional->shadow.height = 32;
        DrawState state;
        for (int group = 0; group < 2; ++group)
        {
            bool cube = (group == 0) == pointsFirst;
            for (int i = 0; i < (cube ? 5 : 9); ++i)
                state.lights.push_back({cube ? static_cast<Light*>(point.get()) : directional.get(),
                    Vector3::ZERO, Vector3::MINUS_ZHAT});
        }
        ShadowPass pass;
        pass.render(state, 12);
        unsigned int cubes = 0, maps = 0;
        std::set<unsigned int> resources;
        for (const auto& instance : state.lights)
        {
            bool cube = instance.light == point.get();
            unsigned int index = cube ? cubes++ : maps++;
            bool selected = index < (cube ? 4u : 8u);
            BOOST_TEST(instance.shadowIndex == (selected ? static_cast<int>(index) : -1));
            BOOST_TEST(static_cast<bool>(instance.shadowMap) == selected);
            if (selected) resources.insert(instance.shadowMap->id());
        }
        BOOST_TEST(resources.size() == 12u);
    }
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
    for (unsigned int i = 0; i < MAX_LIGHTS - 1; ++i) {
        auto point = PointLight::create();
        point->shadow.enabled = false;
        scene->add(point);
    }
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

/**
 * Tests allocation of a point-light depth cubemap and assignment of its shadow sampler index.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_pointDepthCube)
{
    auto light = PointLight::create();
    light->shadow.width = light->shadow.height = 32;
    DrawState state;
    state.lights.push_back({light.get(), Vector3(2.f, 3.f, 4.f)});
    ShadowPass pass;
    pass.render(state, 1);
    BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
    BOOST_TEST(state.lights[0].shadowMap->type() == GL_TEXTURE_CUBE_MAP);
    BOOST_TEST(state.lights[0].shadowMap->width() == 32u);
    BOOST_CHECK(state.lights[0].shadowMap->format() == PixelFormat::DEPTH32F);
    BOOST_TEST(state.lights[0].shadowIndex == 0);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Tests restoration of the active texture unit and the previous cubemap binding after point-shadow rendering.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_pointRestoresCubeBinding)
{
    TextureCubeConfig config;
    auto previous = TextureCube::create(config);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, previous->id());
    glActiveTexture(GL_TEXTURE2);
    auto light = PointLight::create();
    light->shadow.width = light->shadow.height = 32;
    DrawState state;
    state.lights.push_back({light.get()});
    ShadowPass pass;
    pass.render(state, 1);
    GLint active, binding;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    BOOST_TEST(active == GL_TEXTURE2);
    glActiveTexture(GL_TEXTURE0);
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &binding);
    BOOST_TEST(binding == previous->id());
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/**
 * Tests the center and up orientation of all six cubemap face transforms and the uploaded far plane.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_pointFaceTransforms)
{
    auto light = PointLight::create();
    light->shadow.width = light->shadow.height = 32;
    light->shadow.far = 20.f;
    const Vector3 position(2.f, 3.f, 4.f);
    DrawState state;
    state.lights.push_back({light.get(), position});
    ShadowPass pass;
    pass.render(state, 1);
    auto shader = ShaderManager::getShader(ShaderId::SHADOW_DEPTH);
    const Vector3 directions[] = {Vector3::XHAT, Vector3::MINUS_XHAT, Vector3::YHAT,
        Vector3::MINUS_YHAT, Vector3::ZHAT, Vector3::MINUS_ZHAT};
    const Vector3 ups[] = {Vector3::MINUS_YHAT, Vector3::MINUS_YHAT, Vector3::ZHAT,
        Vector3::MINUS_ZHAT, Vector3::MINUS_YHAT, Vector3::MINUS_YHAT};
    for (int face = 0; face < 6; ++face)
    {
        float matrix[16];
        auto name = "faceTransforms[" + std::to_string(face) + "]";
        GLint location = glGetUniformLocation(shader->id(), name.c_str());
        BOOST_REQUIRE(location >= 0);
        glGetUniformfv(shader->id(), location, matrix);
        // GLSL matrices are column-major. Face center projects to (0,0), face up to positive Y.
        auto project = [&](const Vector3& p, int row) {
            float w = matrix[3]*p.x + matrix[7]*p.y + matrix[11]*p.z + matrix[15];
            return (matrix[row]*p.x + matrix[4+row]*p.y + matrix[8+row]*p.z + matrix[12+row]) / w;
        };
        auto center = position.plus(directions[face].scale(2.f));
        BOOST_TEST(std::abs(project(center, 0)) < 0.00001f);
        BOOST_TEST(std::abs(project(center, 1)) < 0.00001f);
        BOOST_TEST(std::abs(project(center.plus(ups[face]), 1) - 0.5f) < 0.00001f);
        BOOST_TEST(project(center, 2) > -1.f);
        BOOST_TEST(project(center, 2) < 1.f);
    }
    float farPlane;
    glGetUniformfv(shader->id(), glGetUniformLocation(shader->id(), "farPlane"), &farPlane);
    BOOST_TEST(farPlane == 20.f);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Tests that invalid point-shadow settings leave the light unshadowed without consuming a valid light's slot.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_pointInvalidSettings)
{
    auto point = PointLight::create();
    auto valid = PointLight::create();
    valid->shadow.width = valid->shadow.height = 32;
    ShadowPass pass;
    for (int invalid = 0; invalid < 15; ++invalid)
    {
        point->shadow = valid->shadow;
        Vector3 position;
        switch (invalid) {
            case 0: point->shadow.height = 16; break;
            case 1: point->shadow.width = 0; break;
            case 2: point->shadow.width = point->shadow.height = std::numeric_limits<int>::max(); break;
            case 3: point->shadow.near = 0.f; break;
            case 4: point->shadow.far = point->shadow.near; break;
            case 5: point->shadow.far = std::numeric_limits<float>::infinity(); break;
            case 6: position.x = std::numeric_limits<float>::quiet_NaN(); break;
            case 7: point->shadow.height = -1; break;
            case 8: point->shadow.near = std::numeric_limits<float>::quiet_NaN(); break;
            case 9: point->shadow.far = -1.f; break;
            case 10: position.z = std::numeric_limits<float>::infinity(); break;
            case 12: point->shadow.point.filterRadius = -0.01f; break;
            case 13: point->shadow.point.filterRadius = std::numeric_limits<float>::infinity(); break;
            case 14: point->shadow.point.filterRadius = std::numeric_limits<float>::quiet_NaN(); break;
            case 11:
                point->shadow.near = std::numeric_limits<float>::max() / 4.f;
                point->shadow.far = std::numeric_limits<float>::max() / 2.f;
                break;
        }
        DrawState state;
        state.lights.push_back({point.get(), position});
        state.lights.push_back({valid.get()});
        ShadowDiagnostics diagnostics;
        pass.render(state, 1);
        BOOST_TEST_CONTEXT("invalid setting=" << invalid)
        {
            BOOST_TEST(diagnostics.output.str().find("Point shadow") != std::string::npos);
            BOOST_TEST(diagnostics.output.str().find("without shadows") != std::string::npos);
            if (invalid == 0) BOOST_TEST(diagnostics.output.str().find("square") != std::string::npos);
        }
        BOOST_TEST(state.lights[0].light == point.get());
        BOOST_TEST(state.lights[0].shadowMap == nullptr);
        BOOST_TEST(state.lights[0].shadowIndex == -1);
        BOOST_TEST(state.lights[0].shadowTransform == Matrix4::IDENTITY);
        BOOST_REQUIRE(state.lights[1].shadowMap != nullptr);
        BOOST_TEST(state.lights[1].shadowIndex == 0);
        BOOST_TEST(glGetError() == GL_NO_ERROR);
    }
}

/**
 * Tests mixed point, directional, and spot shadow resources with ordinary and instanced alpha-masked meshes.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_mixedPointAnd2DResources)
{
    auto scene = Scene::create();
    for (auto light : {LightPtr(PointLight::create()), LightPtr(DirectionalLight::create(Vector3::MINUS_ZHAT)),
        LightPtr(SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT))})
    {
        light->shadow.width = light->shadow.height = 32;
        scene->add(light);
    }
    auto material = MeshMaterial::create();
    material->alphaMode = AlphaMode::MASK;
    material->texture = Texture2D::create(Texture2DConfig{});
    auto geometry = BoxGeometry::create(1.f, 1.f, 1.f);
    scene->add(Mesh::create(geometry, material));
    auto instances = InstancedMesh::create(geometry, material);
    instances->addInstance({Vector3(1.f, 2.f, -3.f)});
    scene->add(instances);
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    ShadowPass pass;
    pass.render(state, 3);
    for (unsigned int i = 0; i < 3; ++i) {
        BOOST_REQUIRE(state.lights[i].shadowMap != nullptr);
        BOOST_TEST(state.lights[i].shadowMap->type() == (i == 0 ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D));
    }
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Compatible targets survive repeated frames; resolution/type changes replace and release storage.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_targetLifecycle)
{
    auto point = PointLight::create();
    auto directional = DirectionalLight::create(Vector3::MINUS_ZHAT);
    auto spot = SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT);
    for (auto light : {LightPtr(point), LightPtr(directional), LightPtr(spot)})
        light->shadow.width = light->shadow.height = 32;
    DrawState state;
    state.lights.push_back({point.get()});
    ShadowPass pass;
    pass.render(state, 1);
    BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
    auto original = state.lights[0].shadowMap;
    pass.render(state, 1);
    BOOST_TEST(state.lights[0].shadowMap == original);

    const auto originalId = original->id();
    std::weak_ptr<Texture> released = original;
    original.reset();
    point->shadow.width = point->shadow.height = 64;
    pass.render(state, 1);
    BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
    BOOST_TEST(state.lights[0].shadowMap->width() == 64u);
    BOOST_TEST(released.expired());
    BOOST_TEST(glIsTexture(originalId) == GL_FALSE);

    released = state.lights[0].shadowMap;
    state.lights[0].light = directional.get();
    state.lights[0].direction = Vector3::MINUS_ZHAT;
    pass.render(state, 1);
    BOOST_TEST(released.expired());
    BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
    BOOST_TEST(state.lights[0].shadowMap->type() == GL_TEXTURE_2D);
    BOOST_TEST(state.lights[0].shadowTransform != Matrix4::IDENTITY);
    auto map2D = state.lights[0].shadowMap;
    state.lights[0].light = spot.get();
    pass.render(state, 1);
    BOOST_TEST(state.lights[0].shadowMap == map2D);
    map2D.reset();

    state.lights[0].light = point.get();
    pass.render(state, 1);
    BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
    BOOST_TEST(state.lights[0].shadowMap->type() == GL_TEXTURE_CUBE_MAP);
    BOOST_TEST(state.lights[0].shadowTransform == Matrix4::IDENTITY);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Losing eligibility clears per-frame state and releases cached targets, including surplus slots.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_releasesUnusedTargets)
{
    auto point = PointLight::create();
    point->shadow.width = point->shadow.height = 32;
    ShadowPass pass;
    DrawState state;
    state.lights.push_back({point.get()});
    state.lights.push_back({point.get()});
    pass.render(state, 2);
    BOOST_REQUIRE(state.lights[1].shadowMap != nullptr);
    std::weak_ptr<Texture> surplus = state.lights[1].shadowMap;
    pass.render(state, 1);
    BOOST_TEST(surplus.expired());
    BOOST_TEST(state.lights[1].shadowIndex == -1);
    state.lights.resize(1);

    for (int change = 0; change < 5; ++change)
    {
        point->shadow.enabled = true;
        point->shadow.height = 32;
        state.lights[0].light = point.get();
        pass.render(state, 1);
        BOOST_REQUIRE(state.lights[0].shadowMap != nullptr);
        std::weak_ptr<Texture> released = state.lights[0].shadowMap;
        if (change == 0) point->shadow.enabled = false;
        if (change == 1) point->shadow.height = 16;
        if (change == 2) state.lights[0].light = nullptr;
        ShadowDiagnostics diagnostics;
        pass.render(state, change == 3 ? 0 : 1);
        if (change == 4)
        {
            state.lights.clear();
            pass.clear();
        }
        else
        {
            BOOST_TEST(state.lights[0].shadowMap == nullptr);
            BOOST_TEST(state.lights[0].shadowIndex == -1);
            BOOST_TEST(state.lights[0].shadowTransform == Matrix4::IDENTITY);
        }
        BOOST_TEST(released.expired());
    }
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Success and allocation failure both restore caller state; a failed light leaves the slot for the next light.
 */
BOOST_AUTO_TEST_CASE(ShadowPass_restoresStateAndFallsBackAfterFailure)
{
    ScopedRenderState restore;
    auto draw = FrameBuffer::create();
    auto read = FrameBuffer::create();
    auto texture = Texture2D::create(Texture2DConfig{});
    auto cube = TextureCube::create(TextureCubeConfig{});
    auto scene = Scene::create();
    auto material = MeshMaterial::create();
    material->alphaMode = AlphaMode::MASK;
    material->texture = Texture2D::create(Texture2DConfig{});
    scene->add(Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), material));
    auto point = PointLight::create();
    point->shadow.width = point->shadow.height = 32;
    auto directional = DirectionalLight::create(Vector3::MINUS_ZHAT);
    directional->shadow.width = directional->shadow.height = 32;
    scene->add(point);
    scene->add(directional);
    auto program = ShaderManager::getShader(ShaderId::MESH);
    for (bool fail : {false, true})
    {
        DrawState state;
        scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
        program->activate();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw->id());
        glBindFramebuffer(GL_READ_FRAMEBUFFER, read->id());
        glViewport(3, 5, 17, 19);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id());
        glBindTexture(GL_TEXTURE_CUBE_MAP, cube->id());
        glActiveTexture(GL_TEXTURE2);
        ShadowPass pass;
        ShadowDiagnostics diagnostics;
        if (fail)
        {
            IncompleteFramebuffer failure;
            pass.render(state, 1);
            BOOST_TEST(diagnostics.output.str().find("unsupported") != std::string::npos);
        }
        else pass.render(state, 1);
        const auto selected = fail ? 1u : 0u;
        BOOST_TEST(state.lights[1 - selected].shadowMap == nullptr);
        BOOST_TEST(state.lights[1 - selected].shadowIndex == -1);
        BOOST_TEST(state.lights[1 - selected].shadowTransform == Matrix4::IDENTITY);
        BOOST_TEST(state.lights[1 - selected].light == (fail ? point.get() : static_cast<Light*>(directional.get())));
        BOOST_REQUIRE(state.lights[selected].shadowMap != nullptr);
        BOOST_TEST(state.lights[selected].shadowIndex == 0);
        GLint value;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &value);
        BOOST_TEST(value == draw->id());
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &value);
        BOOST_TEST(value == read->id());
        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        BOOST_TEST(value == program->id());
        glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
        BOOST_TEST(value == GL_TEXTURE2);
        glActiveTexture(GL_TEXTURE0);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &value);
        BOOST_TEST(value == texture->id());
        glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &value);
        BOOST_TEST(value == cube->id());
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        const GLint expected[] = {3, 5, 17, 19};
        BOOST_CHECK_EQUAL_COLLECTIONS(viewport, viewport + 4, expected, expected + 4);
        BOOST_TEST(glGetError() == GL_NO_ERROR);
    }
}
