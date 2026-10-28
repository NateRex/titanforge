#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/windows/Window.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/PointLight.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/lights/DirectionalLight.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/objects/Mesh.h>
#include <graphics/objects/PostProcessing.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/textures/Texture2D.h>
#include <limits>
#include <cmath>

struct PresentationSettings
{
    RendererPtr renderer = GlobalTestFixture::RENDERER;
    Color background = renderer->getBackgroundColor();
    float gamma = renderer->getDisplayGamma();
    ~PresentationSettings()
    {
        renderer->setBackgroundColor(background);
        renderer->setDisplayGamma(gamma);
    }
};

/**
 * Tests point and spot light traversal and rendering across attenuation configurations.
 */
BOOST_AUTO_TEST_CASE(Renderer_localLightConfigurations)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setDisplayGamma(1.f);
    auto camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
    for (const LightPtr& light : {LightPtr(PointLight::create()), LightPtr(SpotLight::create())})
    {
        if (light->lightType == LightType::SPOTLIGHT)
        {
            auto spot = std::static_pointer_cast<SpotLight>(light);
            spot->innerAngle = 45.f;
            spot->outerAngle = 60.f;
        }
        auto scene = Scene::create();
        auto material = MeshMaterial::create();
        material->reflectivity = 0.f;
        auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
        mesh->setPosition(0.f, 0.f, -3.5f); // Front face at z=-3, normal +Z.
        scene->add(mesh);
        scene->add(light);
        auto check = [&](float distance)
        {
            light->setPosition(0.f, 0.f, -3.f + distance);
            DrawState state;
            scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
            BOOST_REQUIRE_EQUAL(state.lights.size(), 1u);
            BOOST_TEST(state.lights[0].light == light.get());
            BOOST_TEST(state.lights[0].position == Vector3(0.f, 0.f, -3.f + distance));
            BOOST_REQUIRE_EQUAL(state.items.size(), 1u);
            BOOST_TEST(state.items[0].material == material.get());
            BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
        };
        light->intensity = 0.25f;
        // Exercise distance changes, extreme clamps, and both attenuation models.
        check(1.f);
        check(2.f);
        check(4.f);
        light->minimumDistance = std::numeric_limits<float>::denorm_min();
        check(2.f);
        light->minimumDistance = std::numeric_limits<float>::max();
        check(2.f);
        light->minimumDistance = 0.01f;
        light->range = 0.f; // Range must not affect inverse-square falloff.
        check(1.f);
        light->intensity = 0.5f;
        check(1.f);
        light->intensity = -1.f;
        check(1.f);
        light->intensity = 0.25f;
        light->minimumDistance = 2.f;
        check(1.f);
        check(0.05f);
        light->minimumDistance = 0.01f;
        light->intensity = 0.000025f;
        check(0.005f);
        light->intensity = 0.25f;
        light->attenuationModel = AttenuationModel::RANGE_BASED;
        light->range = 2.f;
        check(2.f);
        check(4.f);
        for (float range : {0.f, -1.f})
        {
            light->range = range;
            check(2.f);
        }
        for (auto model : {AttenuationModel::INVERSE_SQUARE, AttenuationModel::RANGE_BASED})
        {
            light->attenuationModel = model;
            light->attenuation = false;
            light->range = 2.f;
            check(1.f);
            check(4.f);
        }
    }
}

/**
 * Tests ambient and directional light traversal across positions and attenuation settings.
 */
BOOST_AUTO_TEST_CASE(Renderer_ambientAndDirectionalLightConfigurations)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setDisplayGamma(1.f);
    auto camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
    for (const LightPtr& light : {LightPtr(AmbientLight::create()), LightPtr(DirectionalLight::create(Vector3::MINUS_ZHAT))})
    {
        auto scene = Scene::create();
        auto material = MeshMaterial::create();
        material->reflectivity = 0.f;
        auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
        mesh->setPosition(0.f, 0.f, -3.5f);
        scene->add(mesh);
        scene->add(light);
        light->intensity = 0.25f;
        light->attenuation = true; // Even explicitly enabled attenuation cannot affect these types.
        light->range = 1.f;
        light->minimumDistance = 10.f;
        for (auto model : {AttenuationModel::INVERSE_SQUARE, AttenuationModel::RANGE_BASED})
        {
            light->attenuationModel = model;
            for (float z : {-3.5f, -5.5f})
            {
                mesh->setPosition(0.f, 0.f, z);
                renderer->render(scene, camera);
                DrawState state;
                scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
                BOOST_REQUIRE_EQUAL(state.lights.size(), 1u);
                BOOST_TEST(state.lights[0].light == light.get());
                BOOST_REQUIRE_EQUAL(state.items.size(), 1u);
                BOOST_TEST(state.items[0].material == material.get());
            }
        }
    }
}

/**
 * Tests rendering coincident lights with postprocessing and releasing temporary textures.
 */
BOOST_AUTO_TEST_CASE(Renderer_coincidentLightWithPostProcessing)
{
    struct ContextRestore
    {
        GLFWwindow* previous = glfwGetCurrentContext();
        ~ContextRestore() { glfwMakeContextCurrent(previous); }
    } restore;
    ShaderManager::reset();
    // Odd dimensions put the center fragment exactly on the light's world position.
    auto window = Window::create("Light at surface test", 33, 33, WindowFlags::NONE);
    auto renderer = Renderer::create(window);
    struct ShaderCacheReset
    {
        ~ShaderCacheReset() { ShaderManager::reset(); }
    } reset;
    auto camera = PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f);
    for (const LightPtr& light : {LightPtr(PointLight::create()), LightPtr(SpotLight::create())})
    {
        auto scene = Scene::create();
        auto material = MeshMaterial::create();
        material->reflectivity = 0.f;
        auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
        mesh->setPosition(0.f, 0.f, -3.5f);
        scene->add(mesh);
        scene->add(light);
        light->setPosition(0.f, 0.f, -3.f);
        light->intensity = 0.000025f; // Clamped attenuation 10000 => linear 0.25.
        auto effect = PostProcessMaterial::create();
        effect->contrast = 0.5f;
        scene->add(PostProcessing::create(effect));
        renderer->render(scene, camera);
        BOOST_TEST(effect->texture == nullptr);
        DrawState state;
        scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
        BOOST_REQUIRE_EQUAL(state.lights.size(), 1u);
        BOOST_TEST(state.lights[0].position == Vector3(0.f, 0.f, -3.f));
        BOOST_REQUIRE_EQUAL(state.postProcessing.size(), 1u);
        BOOST_TEST(state.postProcessing[0] == effect.get());
    }
}

/**
 * Tests the ability to get the current renderer time
 */
BOOST_AUTO_TEST_CASE(Renderer_getTime)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	BOOST_TEST(renderer->getTime() > 0.0);
}

/**
 * Tests the ability to get the current dimensions of the window
 */
BOOST_AUTO_TEST_CASE(Renderer_getWindowDimensions)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;

	float width, height;
	renderer->getWindowDimensions(&width, &height);
	BOOST_TEST(width == 800);
	BOOST_TEST(height == 600);
}

/**
 * Tests the ability to set and get the background color
 */
BOOST_AUTO_TEST_CASE(Renderer_setBackgroundColor)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	Color oldBackground = renderer->getBackgroundColor();

	renderer->setBackgroundColor(Color::BLUE);
	BOOST_TEST(renderer->getBackgroundColor() == Color::BLUE);

	renderer->setBackgroundColor(oldBackground);
}

/**
 * Tests the ability to render an entity with lighting
 */
BOOST_AUTO_TEST_CASE(Renderer_render)
{
	// Create scene
	ScenePtr scene = Scene::create();

	// Create camera
	CameraPtr camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);

	// Create simple box mesh and add it to the scene
	GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
	MeshMaterialPtr material = MeshMaterial::create();
	MeshPtr mesh = Mesh::create(geometry, material);
	scene->add(mesh);

	// Render
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	BOOST_REQUIRE_NO_THROW((renderer->render(scene, camera,
		RenderModes::MATERIAL |
		RenderModes::VERTEX_NORMALS)));
}

/**
 * Tests the convenience path that renders a scene through a post-process material
 */
BOOST_AUTO_TEST_CASE(Renderer_renderWithPostProcessing)
{
	ScenePtr scene = Scene::create();
	CameraPtr camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
	PostProcessMaterialPtr first = PostProcessMaterial::create();
	first->exposure = 1.2f;
	PostProcessMaterialPtr second = PostProcessMaterial::create();
	second->contrast = 1.15f;
	scene->add(PostProcessing::create(first));
	scene->add(PostProcessing::create(second));

	RendererPtr renderer = GlobalTestFixture::RENDERER;
	BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));

	// Ensure the renderer supplies intermediate textures only for the duration of each draw.
	BOOST_TEST(first->texture == nullptr);
	BOOST_TEST(second->texture == nullptr);
}

/**
 * Tests that rendering preserves the configured linear background and alpha
 */
BOOST_AUTO_TEST_CASE(Renderer_preservesLinearBackground)
{
    auto renderer = GlobalTestFixture::RENDERER;
    PresentationSettings restore;
    renderer->setBackgroundColor(Color(0.0031308f, 0.18f, 0.5f, 0.4f));
    renderer->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
    BOOST_TEST(renderer->getBackgroundColor() == Color(0.0031308f, 0.18f, 0.5f, 0.4f));
}


/**
 * Tests the neutral display gamma default and rejection of non-positive or non-finite settings
 */
BOOST_AUTO_TEST_CASE(Renderer_displayGammaValidation)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    BOOST_TEST(renderer->getDisplayGamma() == 1.f);
    renderer->setDisplayGamma(2.f);
    for (float invalid : {0.f, -1.f, std::numeric_limits<float>::infinity(),
                          -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()})
    {
        BOOST_CHECK_THROW(renderer->setDisplayGamma(invalid), IllegalArgumentException);
        BOOST_TEST(renderer->getDisplayGamma() == 2.f);
    }
}

/**
 * Tests display gamma settings and postprocess cleanup with either framebuffer sRGB state
 */
BOOST_AUTO_TEST_CASE(Renderer_displayGammaWithPostProcessing)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color(0.25f, 0.25f, 0.25f, 0.4f));
    auto camera = PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f);
    auto scene = Scene::create();
    renderer->setDisplayGamma(2.f);
    renderer->render(scene, camera);
    BOOST_TEST(renderer->getDisplayGamma() == 2.f);

    auto exposure = PostProcessMaterial::create();
    exposure->exposure = 2.f;
    auto contrast = PostProcessMaterial::create();
    contrast->contrast = 0.5f;
    scene->add(PostProcessing::create(exposure));
    scene->add(PostProcessing::create(contrast));
    for (bool srgbEnabled : {false, true})
    {
        if (srgbEnabled) glEnable(GL_FRAMEBUFFER_SRGB);
        else glDisable(GL_FRAMEBUFFER_SRGB);
        renderer->setDisplayGamma(1.f);
        renderer->render(scene, camera);
        BOOST_TEST(renderer->getDisplayGamma() == 1.f);
        renderer->setDisplayGamma(2.f);
        BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
        BOOST_TEST(renderer->getDisplayGamma() == 2.f);
        BOOST_TEST(renderer->getBackgroundColor() == Color(0.25f, 0.25f, 0.25f, 0.4f));
        BOOST_TEST(exposure->texture == nullptr);
        BOOST_TEST(contrast->texture == nullptr);
    }
    BOOST_TEST(exposure->texture == nullptr);
    BOOST_TEST(contrast->texture == nullptr);
}

/**
 * Tests effect traversal order and cleanup with contrast adjustments
 */
BOOST_AUTO_TEST_CASE(Renderer_postProcessingOrderAndCleanup)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color(0.f, 0.5f, 1.f));
    auto scene = Scene::create();
    auto contrast = PostProcessMaterial::create();
    contrast->contrast = 3.f;
    auto undoContrast = PostProcessMaterial::create();
    undoContrast->contrast = 0.5f;
    scene->add(PostProcessing::create(contrast));
    scene->add(PostProcessing::create(undoContrast));
    renderer->render(scene, PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.postProcessing.size(), 2u);
    BOOST_TEST(state.postProcessing[0] == contrast.get());
    BOOST_TEST(state.postProcessing[1] == undoContrast.get());
    BOOST_TEST(contrast->texture == nullptr);
    BOOST_TEST(undoContrast->texture == nullptr);
}

/**
 * Tests that translucent materials enter the transparent draw layer
 */
BOOST_AUTO_TEST_CASE(Renderer_transparentMaterialTraversal)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color::BLACK);
    auto scene = Scene::create();
    auto light = AmbientLight::create();
    light->intensity = 1.f;
    scene->add(light);
    auto material = MeshMaterial::create();
    material->color = Color(1.f, 1.f, 1.f, 0.5f);
    auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
    mesh->setPosition(0.f, 0.f, -3.f);
    scene->add(mesh);
    renderer->render(scene, PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f));
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.items.size(), 1u);
    const auto variant = state.items[0].variant(RenderModes::MATERIAL);
    BOOST_REQUIRE(variant != nullptr);
    BOOST_TEST(variant->layer == DrawLayer::TRANSPARENT);
    BOOST_TEST(material->color == Color(1.f, 1.f, 1.f, 0.5f));
}

/**
 * Tests an anti-aliasing window request and shader cache isolation between contexts
 */
BOOST_AUTO_TEST_CASE(Renderer_antiAliasingContextIsolation)
{
    // Prime the global shader cache in the fixture context so this test also
    // catches context isolation failures when run on its own.
    GlobalTestFixture::RENDERER->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));

    {
        struct ContextRestore
        {
            GLFWwindow* previous = glfwGetCurrentContext();
            ~ContextRestore() { glfwMakeContextCurrent(previous); }
        } restore;

        // ShaderManager is global, but program and VAO names belong to a context.
        // Release fixture shaders while their context is still current.
        ShaderManager::reset();
        auto window = Window::create("Linear presentation test", 32, 32, WindowFlags::ANTI_ALIASING);
        auto renderer = Renderer::create(window);
        struct ShaderCacheReset
        {
            // Runs before renderer/window destruction, while the test context is current.
            ~ShaderCacheReset() { ShaderManager::reset(); }
        } resetTestShaders;

        // Anti-aliasing is a request; headless drivers may fall back to one sample.
        BOOST_TEST(renderer->getWindow() == window);
        renderer->setBackgroundColor(Color(0.18f, 0.18f, 0.18f, 0.4f));
        BOOST_REQUIRE_NO_THROW(renderer->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f)));
        BOOST_TEST(renderer->getBackgroundColor() == Color(0.18f, 0.18f, 0.18f, 0.4f));
    }

    // Verify the fixture can recreate and use its own shaders after restoration.
    BOOST_REQUIRE_NO_THROW(GlobalTestFixture::RENDERER->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f)));
}


/**
 * Tests acceptance of extreme finite positive gamma values and preservation of background settings
 */
BOOST_AUTO_TEST_CASE(Renderer_extremeDisplayGamma)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color(0.f, 0.5f, 1.f, 0.4f));
    for (float gamma : {std::numeric_limits<float>::denorm_min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()})
    {
        renderer->setDisplayGamma(gamma);
        renderer->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
        BOOST_TEST(renderer->getDisplayGamma() == gamma);
        BOOST_TEST(renderer->getBackgroundColor() == Color(0.f, 0.5f, 1.f, 0.4f));
    }
}

/**
 * Tests equivalent linear and explicit sRGB input configuration across backgrounds and meshes
 */
BOOST_AUTO_TEST_CASE(Renderer_linearAndSRGBInputConfiguration)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setDisplayGamma(1.f);
    const Color linear(0.2158605f, 0.05126946f, 1.f, 128.f / 255.f);
    const Color srgb = Color::fromSRGBHex("#8040ff80");
    auto camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
    auto scene = Scene::create();

    // Background settings preserve alpha independently of material opacity rules.
    for (const auto& color : {linear, srgb})
    {
        renderer->setBackgroundColor(color);
        renderer->render(scene, camera);
        BOOST_TEST(renderer->getBackgroundColor() == color);
        BOOST_TEST(std::abs(color.red() - linear.red()) < 0.000001f);
        BOOST_TEST(std::abs(color.green() - linear.green()) < 0.000001f);
        BOOST_TEST(color.blue() == linear.blue());
        BOOST_TEST(color.alpha() == linear.alpha());
    }

    renderer->setBackgroundColor(Color::BLACK);
    auto light = AmbientLight::create();
    light->intensity = 1.f;
    scene->add(light);
    auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
    const float positions[] = {-1.f, -1.f, -3.f, 1.f, -1.f, -3.f, 0.f, 1.f, -3.f};
    const float normals[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f};
    geometry->setVertices(positions, 3);
    geometry->setNormals(normals, 3);
    auto material = MeshMaterial::create();
    scene->add(Mesh::create(geometry, material));
    for (bool vertices : {false, true})
    {
        material->useVertexColors = vertices;
        for (const auto& color : {linear, srgb})
        {
            material->color = vertices ? Color::WHITE : color.withAlpha(1.f);
            const float colors[] = {
                color.red(), color.green(), color.blue(), 1.f,
                color.red(), color.green(), color.blue(), 1.f,
                color.red(), color.green(), color.blue(), 1.f};
            geometry->setColors(colors, 3);
            renderer->render(scene, camera);
            DrawState state;
            scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
            BOOST_REQUIRE_EQUAL(state.items.size(), 1u);
            BOOST_TEST(state.items[0].material == material.get());
            BOOST_TEST(material->useVertexColors == vertices);
            BOOST_TEST(material->color == (vertices ? Color::WHITE : color.withAlpha(1.f)));
        }
    }
}

/**
 * Tests mixed linear/sRGB offscreen storage and composition through engine objects.
 * Pixel conversion, blending, and resolve output are outside this structural test.
 */
BOOST_AUTO_TEST_CASE(Renderer_offscreenTextureComposition)
{
    for (unsigned int samples : {1u, 2u})
    {
        BOOST_TEST_CONTEXT("samples=" << samples)
        {
            RenderTargetConfig config;
            config.sizeMode = TargetSizeMode::FIXED;
            config.width = config.height = 4;
            config.colorFormats = {PixelFormat::RGBA8, PixelFormat::SRGBA8};
            config.depthStencilStorage = DepthStencilStorage::NONE;
            config.samples = samples;
            auto target = RenderTarget::create(config);
            BOOST_REQUIRE(target->frameBuffer() != nullptr);
            BOOST_TEST(target->frameBuffer()->isComplete());
            BOOST_REQUIRE_EQUAL(target->colorTextures().size(), 2u);
            BOOST_TEST(target->depthStencilTexture() == nullptr);
            BOOST_TEST(target->depthStencilRenderBuffer() == nullptr);
            BOOST_TEST(target->config().samples == samples);

            for (unsigned int i = 0; i < config.colorFormats.size(); ++i)
            {
                BOOST_TEST_CONTEXT("attachment=" << i)
                {
                    auto texture = std::dynamic_pointer_cast<Texture2D>(target->colorTexture(i));
                    BOOST_REQUIRE(texture != nullptr);
                    BOOST_TEST(texture->format() == config.colorFormats[i]);
                    BOOST_TEST(texture->width() == 4u);
                    BOOST_TEST(texture->height() == 4u);
                    BOOST_TEST(texture->config().samples == samples);

                    // Mesh materials consume regular textures. Multisample storage
                    // requires a resolve before composition, which is not performed here.
                    if (samples > 1) continue;

                    auto scene = Scene::create();
                    auto light = AmbientLight::create();
                    scene->add(light);
                    auto material = MeshMaterial::create();
                    material->texture = texture;
                    material->alphaMode = AlphaMode::OPAQUE;
                    auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
                    mesh->setPosition(0.f, 0.f, -3.f);
                    scene->add(mesh);
                    auto effect = PostProcessMaterial::create();
                    effect->exposure = 0.5f;

                    for (bool effects : {false, true})
                    {
                        if (effects) scene->add(PostProcessing::create(effect));
                        DrawState state;
                        scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
                        BOOST_REQUIRE_EQUAL(state.items.size(), 1u);
                        BOOST_TEST(state.items[0].material == material.get());
                        BOOST_TEST(state.items[0].geometryBuffer != nullptr);
                        const auto variant = state.items[0].variant(RenderModes::MATERIAL);
                        BOOST_REQUIRE(variant != nullptr);
                        BOOST_TEST(variant->layer == DrawLayer::OPAQUE);
                        BOOST_TEST(material->texture == target->colorTexture(i));
                        BOOST_REQUIRE_EQUAL(state.lights.size(), 1u);
                        BOOST_TEST(state.lights[0].light == light.get());
                        BOOST_REQUIRE_EQUAL(state.postProcessing.size(), effects ? 1u : 0u);
                        if (effects)
                        {
                            BOOST_TEST(state.postProcessing[0] == effect.get());
                            BOOST_TEST(state.postProcessing[0]->exposure == 0.5f);
                        }
                        BOOST_TEST(effect->texture == nullptr);
                    }

                    // Resizing preserves the attachment held by the material.
                    target->resize(8, 6);
                    BOOST_TEST(material->texture == target->colorTexture(i));
                    BOOST_TEST(material->texture->width() == 8u);
                    BOOST_TEST(material->texture->height() == 6u);
                    BOOST_TEST(material->texture->format() == config.colorFormats[i]);
                    BOOST_TEST(target->frameBuffer()->isComplete());
                    target->resize(4, 4);
                }
            }
        }
    }
}
