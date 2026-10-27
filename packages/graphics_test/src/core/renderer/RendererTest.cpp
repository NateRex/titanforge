#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/windows/Window.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
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
#include <array>
#include <limits>
#include <thread>
#include <chrono>

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

std::array<unsigned char, 4> presentedPixel(int x = 0, int y = 0)
{
    std::array<unsigned char, 4> pixel;
    glReadBuffer(GL_FRONT);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel.data());
    glReadBuffer(GL_BACK);
    BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
    return pixel;
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
	BOOST_TEST(width = 800);
	BOOST_TEST(height = 600);
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
 * Tests standard sRGB presentation of linear background colors without changing alpha
 */
BOOST_AUTO_TEST_CASE(Renderer_linearPresentation)
{
    auto renderer = GlobalTestFixture::RENDERER;
    PresentationSettings restore;
    renderer->setBackgroundColor(Color(0.0031308f, 0.18f, 0.5f, 0.4f));
    renderer->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
    const auto pixel = presentedPixel();
    BOOST_TEST(std::abs(int(pixel[0]) - 10) <= 2);
    BOOST_TEST(std::abs(int(pixel[1]) - 118) <= 2);
    BOOST_TEST(std::abs(int(pixel[2]) - 188) <= 2);
    BOOST_TEST(std::abs(int(pixel[3]) - 102) <= 2);
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
 * Tests display gamma after linear postprocessing and before sRGB encoding, preserving alpha
 * regardless of the framebuffer sRGB state
 */
BOOST_AUTO_TEST_CASE(Renderer_displayGammaAfterLinearEffects)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color(0.25f, 0.25f, 0.25f, 0.4f));
    auto camera = PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f);
    auto scene = Scene::create();
    renderer->setDisplayGamma(2.f);
    renderer->render(scene, camera);
    BOOST_TEST(std::abs(int(presentedPixel()[0]) - 188) <= 2); // sRGB(sqrt(0.25))

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
        auto pixel = presentedPixel();
        BOOST_TEST(std::abs(int(pixel[0]) - 188) <= 2); // 0.25 * 2, then contrast => 0.5
        BOOST_TEST(std::abs(int(pixel[3]) - 102) <= 2);
        renderer->setDisplayGamma(2.f);
        renderer->render(scene, camera);
        pixel = presentedPixel();
        BOOST_TEST(std::abs(int(pixel[0]) - 219) <= 2); // sRGB(sqrt(0.5))
        BOOST_TEST(std::abs(int(pixel[3]) - 102) <= 2);
    }
    BOOST_TEST(exposure->texture == nullptr);
    BOOST_TEST(contrast->texture == nullptr);
}

/**
 * Tests that intermediate effects retain out-of-range RGB values until final presentation clamps them
 */
BOOST_AUTO_TEST_CASE(Renderer_clampsOnlyAtPresentation)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setBackgroundColor(Color(0.f, 0.5f, 1.f));
    auto scene = Scene::create();
    auto contrast = PostProcessMaterial::create();
    contrast->contrast = 3.f; // [-1, 0.5, 2] must survive intermediate storage.
    auto undoContrast = PostProcessMaterial::create();
    undoContrast->contrast = 0.5f; // [-0.25, 0.5, 1.25], clamped at presentation.
    scene->add(PostProcessing::create(contrast));
    scene->add(PostProcessing::create(undoContrast));
    renderer->render(scene, PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
    const auto pixel = presentedPixel();
    BOOST_TEST(int(pixel[0]) <= 2);
    BOOST_TEST(std::abs(int(pixel[1]) - 188) <= 2);
    BOOST_TEST(int(pixel[2]) >= 253);
}

/**
 * Tests transparent blending in linear RGB before final sRGB encoding
 */
BOOST_AUTO_TEST_CASE(Renderer_blendsInLinearSpace)
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
    const auto pixel = presentedPixel(400, 300);
    BOOST_TEST(std::abs(int(pixel[0]) - 188) <= 2); // half white over black is linear 0.5
    BOOST_TEST(std::abs(int(pixel[1]) - 188) <= 2);
    BOOST_TEST(std::abs(int(pixel[2]) - 188) <= 2);
}

/**
 * Tests linear color presentation with multisampling and shader cache isolation between contexts
 */
BOOST_AUTO_TEST_CASE(Renderer_multisamplePresentation)
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

        GLint samples = 0;
        glGetIntegerv(GL_SAMPLES, &samples);
        BOOST_REQUIRE(samples > 1);
        BOOST_TEST_CONTEXT("Actual default framebuffer samples: " << samples)
        {
            renderer->setBackgroundColor(Color(0.18f, 0.18f, 0.18f, 0.4f));
            renderer->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
            const auto pixel = presentedPixel();
            BOOST_TEST(std::abs(int(pixel[0]) - 118) <= 2);
            BOOST_TEST(std::abs(int(pixel[3]) - 102) <= 2);
        }
    }

    // Verify the fixture can recreate and use its own shaders after restoration.
    GlobalTestFixture::RENDERER->render(Scene::create(), PerspectiveCamera::create(30.f, 1.f, 0.1f, 100.f));
    BOOST_CHECK_NO_THROW(presentedPixel());
}


/**
 * Tests extreme finite positive display gamma values while preserving endpoints and alpha
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
        const auto pixel = presentedPixel();
        BOOST_TEST(int(pixel[0]) <= 2);
        BOOST_TEST(int(pixel[2]) >= 253);
        BOOST_TEST(std::abs(int(pixel[3]) - 102) <= 2);
        if (gamma < 1.f) BOOST_TEST(int(pixel[1]) <= 2);
        else BOOST_TEST(int(pixel[1]) >= 253);
    }
}

/**
 * Tests equivalent displayed output from linear and explicit sRGB background, material, and vertex colors
 */
BOOST_AUTO_TEST_CASE(Renderer_equivalentLinearAndSRGBInputs)
{
    PresentationSettings restore;
    auto renderer = restore.renderer;
    renderer->setDisplayGamma(1.f);
    const Color linear(0.2158605f, 0.05126946f, 1.f, 128.f / 255.f);
    const Color srgb = Color::fromSRGBHex("#8040ff80");
    auto camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
    auto scene = Scene::create();

    // Background exercises alpha without blending or material opacity rules.
    for (const auto& color : {linear, srgb})
    {
        renderer->setBackgroundColor(color);
        renderer->render(scene, camera);
        auto pixel = presentedPixel();
        for (unsigned int i = 0; i < 4; ++i)
            BOOST_TEST(std::abs(int(pixel[i]) - std::array<int, 4>{128, 64, 255, 128}[i]) <= 2);
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
            const auto pixel = presentedPixel(400, 300);
            BOOST_TEST(std::abs(int(pixel[0]) - 128) <= 2);
            BOOST_TEST(std::abs(int(pixel[1]) - 64) <= 2);
            BOOST_TEST(std::abs(int(pixel[2]) - 255) <= 2);
            BOOST_TEST(std::abs(int(pixel[3]) - 255) <= 2);
        }
    }
}
