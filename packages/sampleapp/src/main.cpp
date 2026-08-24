#include <graphics/core/renderer/Renderer.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/objects/Skybox.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/core/pointers/EntityPtr.h>
#include <graphics/loaders/ModelLoader.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/modifiers/InputModifiers.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <common/Constants.h>
#include <common/Utils.h>
#include <cmath>

/**
 * Creates a camera capable of being controlled via key and mouse actions
 * @param renderer The renderer
 * @return The camera that was created
 */
CameraPtr setupCameraMovement(RendererPtr renderer)
{
    PerspectiveCameraPtr camera = PerspectiveCamera::create(45.f, 800.f / 600.f, 0.1f, 100.f);

    InputAction quit("Quit", InputValueType::BOOLEAN);
    InputAction move("Move", InputValueType::VECTOR_2D);
    InputAction look("Look", InputValueType::VECTOR_2D);
    InputAction zoom("Zoom", InputValueType::SCALAR);

    // Create key bindings
    InputContextPtr context = InputContext::create();
    context->add(DigitalInput::KEY_ESCAPE, InputTrigger::RELEASED, quit);
    context->add(DigitalInput::KEY_D, InputTrigger::HELD, move);
    context->add(DigitalInput::KEY_A, InputTrigger::HELD, move, InputModifiers().negateX());
    context->add(DigitalInput::KEY_W, InputTrigger::HELD, move, InputModifiers().swizzle(Axis::Y, Axis::X, Axis::Z));
    context->add(DigitalInput::KEY_S, InputTrigger::HELD, move, InputModifiers().negateX().swizzle(Axis::Y, Axis::X, Axis::Z));
    context->add(AxisInput::MOUSE_MOVE, look, InputModifiers().negateY());
    context->add(AxisInput::MOUSE_SCROLL, zoom, InputModifiers().swizzle(Axis::Y, Axis::X, Axis::Z));

    WindowPtr window = renderer->getWindow();
    InputController* inputController = window->getInputController();
    inputController->addContext(context);

    // Bind quit action
    inputController->bind(quit, [renderer](InputValue value, float deltaTime)
    {
        renderer->destroy(true);
    });

    // Bind move action
    inputController->bind(move, [camera](InputValue value, float deltaTime)
    {
        Vector2 v = value.get2D();
        float cameraSpeed = 2.5f * deltaTime;
        
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

    return camera;
}

/**
 * Main entrypoint for the application
 */
int main()
{
    RendererPtr renderer = Renderer::create();
    CameraPtr camera = setupCameraMovement(renderer);
    camera->lookAt(Vector3(0.f, 0.f, 10.f), Vector3::ZERO, Vector3::YHAT);

    ScenePtr scene = Scene::create();

    // Create skybox
    SkyboxMaterialPtr skyboxMat = SkyboxMaterial::create();
    skyboxMat->texture = TextureCube::create({
        "assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
    });
    SkyboxPtr skybox = Skybox::create(skyboxMat);
    scene->add(skybox);

    // Load guitar backpack model
    EntityPtr entity = ModelLoader::load("assets/backpack/backpack.obj");
    entity->setPosition(0.f, 0.f, 0.f);
    scene->add(entity);

    // Create lighting
    LightPtr ambientLighting = AmbientLight::create();
    ambientLighting->intensity = 1.f;
    scene->add(ambientLighting);

    // Create post-processing effects
    PostProcessMaterialPtr postProcessing = PostProcessMaterial::create();
    postProcessing->contrast = 1.1f;
    postProcessing->saturation = 0.8f;
    postProcessing->exposure = 1.1f;

    float rotationRate = 0.25f;
    while (renderer->getWindow()->isOpen())
    {
        // Rotate entity
        entity->addRotation(Matrix3::fromYRotation(rotationRate * renderer->getDeltaTime()));

        renderer->render(scene, camera, postProcessing);
    }
}
