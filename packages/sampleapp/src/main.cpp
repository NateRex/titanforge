#include <graphics/core/Application.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/modifiers/InputModifiers.h>
#include <graphics/loaders/TextureLoader.h>
#include <graphics/loaders/ModelLoader.h>

#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/DirectionalLight.h>
#include <graphics/cameras/PerspectiveCamera.h>

#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>

#include <graphics/objects/Mesh.h>
#include <graphics/objects/InstancedMesh.h>
#include <graphics/objects/Skybox.h>
#include <graphics/objects/PostProcessing.h>

#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Constants.h>
#include <common/Utils.h>
#include <cmath>
#include <random>

/**
 * Creates a camera capable of being controlled via key and mouse actions
 * @param application Application
 * @return The camera that was created
 */
PerspectiveCameraPtr setupInputs(Application* app)
{
    float width, height;
    RendererPtr renderer = app->getRenderer();
    renderer->getWindowDimensions(&width, &height);
    PerspectiveCameraPtr camera = PerspectiveCamera::create(45.f, width / height, 0.1f, 100.f);

    InputAction quit("Quit", InputValueType::BOOLEAN);
    InputAction move("Move", InputValueType::VECTOR_2D);
    InputAction look("Look", InputValueType::VECTOR_2D);
    InputAction zoom("Zoom", InputValueType::SCALAR);
    InputAction toggleShadows("ToggleShadows", InputValueType::BOOLEAN);

    // Create key bindings
    InputContextPtr context = InputContext::create();
    context->add(DigitalInput::KEY_ESCAPE, InputTrigger::RELEASED, quit);
    context->add(DigitalInput::KEY_D, InputTrigger::HELD, move);
    context->add(DigitalInput::KEY_A, InputTrigger::HELD, move, InputModifiers().negateX());
    context->add(DigitalInput::KEY_W, InputTrigger::HELD, move, InputModifiers().swizzle(Axis::Y, Axis::X, Axis::Z));
    context->add(DigitalInput::KEY_S, InputTrigger::HELD, move, InputModifiers().negateX().swizzle(Axis::Y, Axis::X, Axis::Z));
    context->add(DigitalInput::KEY_1, InputTrigger::PRESSED, toggleShadows);
    context->add(AxisInput::MOUSE_MOVE, look, InputModifiers().negateY());
    context->add(AxisInput::MOUSE_SCROLL, zoom, InputModifiers().swizzle(Axis::Y, Axis::X, Axis::Z));

    InputController* inputController = app->getInputController();
    inputController->addContext(context);

    // Bind quit action
    inputController->bind(quit, [app](InputValue value, float deltaTime)
    {
        app->stop();
    });

    // Bind move action
    inputController->bind(move, [camera](InputValue value, float deltaTime)
    {
        Vector2 v = value.get2D();
        float cameraSpeed = 5.f * deltaTime;
        
        Vector3 sideToSideMovement = camera->getRightVector().scale(cameraSpeed * v.x);
        camera->addPosition(sideToSideMovement);

        Vector3 forwardMovement = camera->getForwardVector().scale(cameraSpeed * v.y);
        camera->addPosition(forwardMovement);
    });

    // Bind look action
    inputController->bind(look, [camera](InputValue value, float deltaTime)
    {
        Vector2 v = value.get2D();
        camera->addYaw(v.x);
        camera->addPitch(v.y);
    });

    // Bind zoom action
    inputController->bind(zoom, [camera](InputValue value, float deltaTime)
    {
        float fov = camera->getFOV();
        fov -= value.getScalar();
        fov = clamp(fov, 1.f, 45.f);
        camera->setFOV(fov);
    });

    // Bind shadow toggle
    inputController->bind(toggleShadows, [renderer](InputValue value, float deltaTime)
    {
        renderer->setShadowsEnabled(!renderer->getShadowsEnabled());
    });

    return camera;
}

/**
 * Creates grass instances distributed around the given plane
 * @param scene The scene
 * @param quantity The number of instances to generate
 * @param fixedY Fixed height along the y-axis
 * @param minX Lower-bounds on the x-axis
 * @param maxX Upper-bounds on the x-axis
 * @param minZ Lower-bounds on the z-axis
 * @param maxZ Upper-bounds on the z-axis
 */
void createGrass(ScenePtr scene, unsigned int quantity, float fixedY, float minX, float maxX, float minZ, float maxZ)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_real_distribution<float> xDist(minX, maxX);
    std::uniform_real_distribution<float> zDist(minZ, maxZ);
    std::uniform_real_distribution<float> rDist(0.f, PI * 2);

    BoxGeometryPtr geom = BoxGeometry::create(1.f, 1.f, 0.05f);
    MeshMaterialPtr mat = MeshMaterial::create();
    mat->texture = TextureLoader::load("assets/grass.png", true);
    mat->alphaMode = AlphaMode::MASK;
    InstancedMeshPtr mesh = InstancedMesh::create(geom, mat);
    scene->add(mesh);

    for (unsigned int i = 0; i < quantity; i++)
    {
        mesh->addInstance({
            Vector3(xDist(g), fixedY, zDist(g)),
            Matrix3::fromYRotation(rDist(g))
        });
    }
}

/**
 * Main entrypoint for the application
 */
int main()
{
    // Create app
    RendererPtr renderer = Renderer::create(nullptr,
        WindowFlags::RESIZABLE |
        WindowFlags::ANTI_ALIASING |
        WindowFlags::VSYNC);
    Application app(renderer);

    // Setup scene and camera
    ScenePtr scene = Scene::create();
    PerspectiveCameraPtr camera = setupInputs(&app);
    camera->lookAt(Vector3(0.f, 5.f, 10.f), Vector3::ZERO, Vector3::YHAT);

    // Create skybox
    scene->add(Skybox::create());

    // Create lighting
    LightPtr ambientLighting = AmbientLight::create();
    ambientLighting->intensity = 0.15f;
    scene->add(ambientLighting);

    // Create directional light simulating sunlight
    DirectionalLightPtr sun = DirectionalLight::create(Vector3(-0.5f, -1.f, -0.3f));
    sun->intensity = 0.65f;
    scene->add(sun);

    // Create post-processing effects
    PostProcessMaterialPtr postProcessing = PostProcessMaterial::create();
    postProcessing->contrast = 1.f;
    postProcessing->saturation = 0.9f;
    postProcessing->exposure = 1.f;
    scene->add(PostProcessing::create(postProcessing));

    // Create the ground
    MeshMaterialPtr groundMat = MeshMaterial::create();
    groundMat->color = Color(0.3f, 0.3f, 0.3f, 1.f);
    scene->add(Mesh::create(BoxGeometry::create(12.f, 1.f, 12.f), groundMat));

    // Create grass
    createGrass(scene, 200, 1.f, -5.f, 5.f, -5.f, 5.f);

    // Load guitar backpack model
    EntityPtr entity = ModelLoader::load("assets/backpack/backpack.obj");
    entity->setPosition(0.f, 2.1f, 0.f);
    entity->addRotation(Matrix3::fromYRotation(0.25f));
    scene->add(entity);

    // Create opaque cube
    MeshPtr cube = Mesh::create(BoxGeometry::create(1.2f, 2.f, 1.2f), MeshMaterial::create());
    cube->setPosition(-2.f, 1.5f, 5.f);
    scene->add(cube);

    // Create glass panel
    MeshMaterialPtr glass = MeshMaterial::create();
    glass->color = Color(0.3f, 0.6f, 0.8f, 0.5f);
    glass->alphaMode = AlphaMode::BLEND;
    glass->cullingMode = CullingMode::NONE;
    MeshPtr glassPanel = Mesh::create(BoxGeometry::create(4.f, 0.05f, 4.f), glass);
    glassPanel->setPosition(-2.f, 1.2f, 5.f);
    scene->add(glassPanel);

    // Run application
    app.run([&](const Frame& frame) {
        float width, height;
        app.getRenderer()->getWindowDimensions(&width, &height);
        camera->setAspect(width / height);

        float time = frame.elapsedTime;
        cube->setPosition(-2.f + std::sin(time), 1.5f, 5.f);

        renderer->render(scene, camera, RenderModes::MATERIAL);
    });
}
