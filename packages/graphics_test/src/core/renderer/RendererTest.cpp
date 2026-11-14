#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
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
#include <sstream>
#include <graphics/objects/InstancedMesh.h>

struct PresentationSettings
{
    RendererPtr renderer = GlobalTestFixture::RENDERER;
    Color background = renderer->getBackgroundColor();
    float gamma = renderer->getDisplayGamma();
    bool shadows = renderer->getShadowsEnabled();
    unsigned int budget = renderer->getShadowBudget();
    ~PresentationSettings()
    {
        renderer->setBackgroundColor(background);
        renderer->setDisplayGamma(gamma);
        renderer->setShadowsEnabled(shadows);
        renderer->setShadowBudget(budget);
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
    auto scene = Scene::create();
    auto camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
    auto material = MeshMaterial::create();
    material->reflectivity = 0.f;
    auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
    mesh->setPosition(0.f, 0.f, -3.5f);
    scene->add(mesh);
    
    // Eight representative configurations of each type fit in the shader's
    // sixteen-light limit, so every configuration participates in this draw.
    for (bool spot : {false, true})
    {
        for (int configuration = 0; configuration < 8; ++configuration)
        {
            LightPtr light = spot ? LightPtr(SpotLight::create()) : LightPtr(PointLight::create());
            light->setPosition(0.f, 0.f, -2.f + configuration * 0.1f);
            light->intensity = configuration == 3 ? -1.f : 0.25f;
            if (configuration == 3)
            {
                light->attenuationModel = AttenuationModel::RANGE_BASED;
                light->range = 2.f;
            }
            if (configuration == 1) light->minimumDistance = std::numeric_limits<float>::denorm_min();
            if (configuration == 2) light->minimumDistance = std::numeric_limits<float>::max();
            if (configuration >= 4) light->attenuationModel = AttenuationModel::RANGE_BASED;
            if (configuration == 4) light->range = 0.f;
            if (configuration == 5) light->range = -1.f;
            if (configuration >= 6) light->attenuation = false;
            if (configuration == 6) light->attenuationModel = AttenuationModel::INVERSE_SQUARE;
            if (spot)
            {
                auto spotlight = std::static_pointer_cast<SpotLight>(light);
                spotlight->innerAngle = 45.f;
                spotlight->outerAngle = 60.f;
            }
            scene->add(light);
        }
    }
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.lights.size(), 16u);
    BOOST_REQUIRE_EQUAL(state.items.size(), 1u);
    BOOST_TEST(state.items[0].material == material.get());
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Tests ambient and directional light traversal across positions and attenuation settings.
 */
BOOST_AUTO_TEST_CASE(Renderer_ambientAndDirectionalLightConfigurations)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    auto scene = Scene::create();
    auto camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
    for (bool directional : {false, true})
    {
        for (auto model : {AttenuationModel::INVERSE_SQUARE, AttenuationModel::RANGE_BASED})
        {
            LightPtr light = directional ? LightPtr(DirectionalLight::create(Vector3::MINUS_ZHAT))
                                         : LightPtr(AmbientLight::create());
            light->intensity = 0.25f;
            light->attenuation = true; // Ignored for these light types.
            light->range = 1.f;
            light->minimumDistance = 10.f;
            light->attenuationModel = model;
            scene->add(light);
        }
    }
    auto material = MeshMaterial::create();
    material->reflectivity = 0.f;
    for (float z : {-3.5f, -5.5f})
    {
        auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
        mesh->setPosition(0.f, 0.f, z);
        scene->add(mesh);
    }
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.lights.size(), 4u);
    BOOST_REQUIRE_EQUAL(state.items.size(), 2u);
    for (const auto& item : state.items) BOOST_TEST(item.material == material.get());
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
    BOOST_TEST(glGetError() == GL_NO_ERROR);
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
    auto scene = Scene::create();
    auto material = MeshMaterial::create();
    material->reflectivity = 0.f;
    auto mesh = Mesh::create(BoxGeometry::create(2.f, 2.f, 1.f), material);
    mesh->setPosition(0.f, 0.f, -3.5f);
    scene->add(mesh);
    for (const LightPtr& light : {LightPtr(PointLight::create()), LightPtr(SpotLight::create())})
    {
        scene->add(light);
        light->setPosition(0.f, 0.f, -3.f);
        light->intensity = 0.000025f; // Clamped attenuation 10000 => linear 0.25.
    }
    auto effect = PostProcessMaterial::create();
    effect->contrast = 0.5f;
    scene->add(PostProcessing::create(effect));
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
    BOOST_TEST(effect->texture == nullptr);
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.lights.size(), 2u);
    for (const auto& light : state.lights)
        BOOST_TEST(light.position == Vector3(0.f, 0.f, -3.f));
    BOOST_REQUIRE_EQUAL(state.postProcessing.size(), 1u);
    BOOST_TEST(state.postProcessing[0] == effect.get());
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
 * Tests display gamma and postprocess cleanup with framebuffer sRGB enabled
 */
BOOST_AUTO_TEST_CASE(Renderer_displayGammaWithPostProcessing)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color(0.25f, 0.25f, 0.25f, 0.4f));
    renderer->setDisplayGamma(2.f);
    auto scene = Scene::create();
    auto exposure = PostProcessMaterial::create();
    exposure->exposure = 2.f;
    auto contrast = PostProcessMaterial::create();
    contrast->contrast = 0.5f;
    scene->add(PostProcessing::create(exposure));
    scene->add(PostProcessing::create(contrast));
    const auto srgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
    struct SrgbRestore
    {
        GLboolean enabled;
        ~SrgbRestore() { if (enabled) glEnable(GL_FRAMEBUFFER_SRGB); else glDisable(GL_FRAMEBUFFER_SRGB); }
    } restoreSrgb{srgbEnabled};
    glEnable(GL_FRAMEBUFFER_SRGB);
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f)));
    BOOST_TEST(renderer->getDisplayGamma() == 2.f);
    BOOST_TEST(renderer->getBackgroundColor() == Color(0.25f, 0.25f, 0.25f, 0.4f));
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
    // Prime fixture shader ownership without rendering a frame.
    const auto previousContext = glfwGetCurrentContext();
    ShaderManager::getShader(ShaderId::MESH);

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

    BOOST_TEST(glfwGetCurrentContext() == previousContext);
    BOOST_REQUIRE_NO_THROW(ShaderManager::getShader(ShaderId::MESH));
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
        BOOST_TEST(renderer->getDisplayGamma() == gamma);
        BOOST_TEST(renderer->getBackgroundColor() == Color(0.f, 0.5f, 1.f, 0.4f));
    }
    BOOST_REQUIRE_NO_THROW(renderer->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f)));
    BOOST_TEST(renderer->getDisplayGamma() == std::numeric_limits<float>::max());
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
    auto scene = Scene::create();
    scene->add(AmbientLight::create());
    renderer->setBackgroundColor(srgb);
    BOOST_TEST(std::abs(srgb.red() - linear.red()) < 0.000001f);
    BOOST_TEST(std::abs(srgb.green() - linear.green()) < 0.000001f);
    BOOST_TEST(srgb.blue() == linear.blue());
    BOOST_TEST(srgb.alpha() == linear.alpha());
    for (bool vertices : {false, true})
    {
        for (const auto& color : {linear, srgb})
        {
            auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
            const float positions[] = {-1.f, -1.f, -3.f, 1.f, -1.f, -3.f, 0.f, 1.f, -3.f};
            const float normals[] = {0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f};
            geometry->setVertices(positions, 3);
            geometry->setNormals(normals, 3);
            const float colors[] = {
                color.red(), color.green(), color.blue(), 1.f,
                color.red(), color.green(), color.blue(), 1.f,
                color.red(), color.green(), color.blue(), 1.f};
            geometry->setColors(colors, 3);
            auto material = MeshMaterial::create();
            material->useVertexColors = vertices;
            material->color = vertices ? Color::WHITE : color.withAlpha(1.f);
            auto mesh = Mesh::create(geometry, material);
            mesh->setPosition(vertices ? 0.5f : -0.5f, 0.f, color == linear ? 0.f : -0.5f);
            scene->add(mesh);
            BOOST_TEST(material->useVertexColors == vertices);
            BOOST_TEST(material->color == (vertices ? Color::WHITE : color.withAlpha(1.f)));
        }
    }
    DrawState state;
    scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
    BOOST_REQUIRE_EQUAL(state.items.size(), 4u);
    BOOST_REQUIRE_EQUAL(state.lights.size(), 1u);
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f)));
    BOOST_TEST(renderer->getBackgroundColor() == srgb);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
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

/**
 * Tests the ability to render a scene containing a mixture of light sources producing shadows, and objects that may or may not cast/receive
 * shadows.
 */
BOOST_AUTO_TEST_CASE(Renderer_mixedPointShadows)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setShadowsEnabled(true);
    renderer->setShadowBudget(3);
    auto scene = Scene::create();
    auto camera = PerspectiveCamera::create(45.f, 1.f, 0.1f, 100.f);
    for (auto light : {LightPtr(PointLight::create()), LightPtr(DirectionalLight::create(Vector3::MINUS_ZHAT)),
        LightPtr(SpotLight::create(Vector3::ZERO, Vector3::MINUS_ZHAT))}) {
        light->shadow.width = light->shadow.height = 32;
        scene->add(light);
    }
    auto geometry = BoxGeometry::create(1.f, 1.f, 1.f);
    auto texture = Texture2D::create(Texture2DConfig{});
    float x = -2.f;
    for (auto mode : {AlphaMode::OPAQUE, AlphaMode::MASK, AlphaMode::BLEND})
    {
        for (bool shadows : {false, true})
        {
            auto material = MeshMaterial::create();
            material->texture = texture;
            material->alphaMode = mode;
            material->castShadow = material->receiveShadow = shadows;
            const float y = shadows ? 1.f : -1.f;
            auto mesh = Mesh::create(geometry, material);
            mesh->setPosition(x, y, -8.f);
            scene->add(mesh);
            auto instances = InstancedMesh::create(geometry, material);
            instances->addInstance({Vector3(x, y, -10.f)});
            scene->add(instances);
        }
        x += 2.f;
    }
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Invalid, disabled and over-capacity shadows preserve direct lighting alongside valid shadow indices.
 */
BOOST_AUTO_TEST_CASE(Renderer_shadowFallbackLifecycle)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setShadowsEnabled(true);
    renderer->setShadowBudget(12);
    auto scene = Scene::create();
    auto camera = PerspectiveCamera::create(45.f, 1.f, 0.1f, 100.f);
    auto material = MeshMaterial::create();
    scene->add(Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), material));
    std::vector<PointLightPtr> points;
    for (int i = 0; i < 6; ++i)
    {
        auto point = PointLight::create();
        point->shadow.width = point->shadow.height = 32;
        point->intensity = 2.f;
        scene->add(point);
        points.push_back(point);
    }
    auto directional = DirectionalLight::create(Vector3::MINUS_ZHAT);
    directional->shadow.width = directional->shadow.height = 32;
    directional->intensity = 2.f;
    scene->add(directional);

    // Mix invalid, disabled, valid and over-capacity point shadows in one draw.
    points[0]->shadow.near = 0.f;
    points[1]->shadow.enabled = false;
    BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera));
    auto shader = ShaderManager::getShader(ShaderId::MESH);
    auto integerUniform = [&](const std::string& name) {
        const auto location = glGetUniformLocation(shader->id(), name.c_str());
        BOOST_REQUIRE(location >= 0);
        GLint value;
        glGetUniformiv(shader->id(), location, &value);
        return value;
    };
    BOOST_TEST(integerUniform("uLightCount") == 7);
    const int expected[] = {-1, -1, 0, 1, 2, -1, 0};
    for (int i = 0; i < 7; ++i)
    {
        const std::string prefix = "uLights[" + std::to_string(i) + "].";
        BOOST_TEST(integerUniform(prefix + "shadowIndex") == expected[i]);
        float intensity;
        glGetUniformfv(shader->id(), glGetUniformLocation(shader->id(), (prefix + "intensity").c_str()), &intensity);
        BOOST_TEST(intensity == 2.f);
    }
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}
