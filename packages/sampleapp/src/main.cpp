#include <graphics/core/Renderer.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/modifiers/InputModifiers.h>
#include <graphics/textures/Texture.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <common/Constants.h>
#include <common/Utils.h>
#include <cmath>

/**
 * Creates and adds a number of boxes to the scene
 * @param scene Scene to add the boxes to
 * @return The newly-created mesh that's been added to the scene
 */
MeshPtr createBoxes(ScenePtr scene)
{
    GeometryPtr geometry = BoxGeometry::create(1, 1, 1);
    MaterialPtr material = BasicMaterial::create();
    material->texture = Texture::create("assets/container2.png");
    material->diffuseMap = material->texture;
    material->specularMap = Texture::create("assets/container2_specular.png");

    struct BoxTransform
    {
        Vector3 position;
        Vector3 rotationAxis;
        float rotationDegrees;
        float scaling;
    };

    const BoxTransform transforms[] = {
        { Vector3(  0.f,  0.f,   0.f), Vector3::YHAT, 0.f, 0.5f },
        { Vector3(  4.f,  2.f,  -3.f), Vector3::XHAT, 25.f, 0.8f },
        { Vector3( -6.f, -2.f,  -5.f), Vector3::YHAT, 40.f, 1.1f },
        { Vector3(  8.f, -1.f,  -7.f), Vector3::ZHAT, 18.f, 1.4f },
        { Vector3( -9.f,  3.f,  -9.f), Vector3(1.f, 1.f, 0.f), 55.f, 1.8f },
        { Vector3( 10.f,  4.f, -11.f), Vector3(0.f, 1.f, 1.f), 70.f, 2.1f },
        { Vector3(-11.f, -4.f, -12.f), Vector3(1.f, 0.f, 1.f), 32.f, 2.5f },
        { Vector3(  6.f,  5.f, -17.f), Vector3(1.f, 1.f, 1.f), 63.f, 2.8f }
    };

    MeshPtr mesh;
    for (const BoxTransform& transform : transforms)
    {
        mesh = Mesh::create(geometry, material);
        mesh->setPosition(transform.position);
        mesh->setRotation(Matrix3::fromRotation(
            transform.rotationAxis,
            deg2Rad(transform.rotationDegrees)));
        mesh->setScaling(transform.scaling);
        scene->add(mesh);
    }

    return mesh;
}

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

    // Create textured boxes
    createBoxes(scene);

    // Create lighting
    LightPtr ambientLighting = AmbientLight::create();
    LightPtr spotLight = SpotLight::create();
    spotLight->intensity = 2.f;
    scene->add(ambientLighting);
    scene->add(spotLight);

    while (renderer->getWindow()->isOpen())
    {
        // Move spotlight with camera
        spotLight->lookAt(camera->getPosition(), camera->getPosition().plus(camera->getForwardVector()));

        renderer->render(scene, camera);
    }
}
