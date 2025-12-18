#include <graphics/core/Renderer.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/PointLight.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/modifiers/InputModifiers.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Utils.h>
#include <cmath>

/**
 * Creates a mesh representing a box
 * @param color Box color
 * @param x Global x position of the box
 * @param y Global y position of the box
 * @param z Global z position of the box
 * @return The newly-created mesh that's been added to the scene
 */
MeshPtr createBox(const Color& color, float x, float y, float z)
{
    GeometryPtr geometry = BoxGeometry::create(1, 1, 1);

    MaterialPtr material = BasicMaterial::create();
    material->color = color;
    
    MeshPtr mesh = Mesh::create(geometry, material);
    mesh->setPosition(x, y, z);
    return mesh;
}

/**
 * Creates a camera capable of being controlled via key and mouse actions
 * @param renderer The renderer
 * @return The camera that was created
 */
CameraPtr createCamera(RendererPtr renderer)
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

    CameraPtr camera = createCamera(renderer);
    camera->lookAt(Vector3(0.f, 0.f, 10.f), Vector3::ZERO, Vector3::YHAT);

    ScenePtr scene = Scene::create();

    // Create colored cube
    MeshPtr colorCube = createBox(Color(1.0f, 0.5f, 0.31f), 0.f, 0.f, 0.f);
    scene->add(colorCube);

    // Create lighting
    LightPtr ambientLighting = AmbientLight::create();
    LightPtr pointLight = PointLight::create();
    pointLight->intensity = 0.5;
    scene->add(ambientLighting);
    scene->add(pointLight);

    float angle = 0.f;
    const float lightRadius = 5.f;
    const float angularSpeed = 1.f;
    while (renderer->getWindow()->isOpen())
    {
        float angleChange = angularSpeed * renderer->getDeltaTime();

        // Rotate light
        angle -= angleChange;
        Vector3 lightPos(lightRadius * cos(angle), 0.f, lightRadius * sin(angle));
        pointLight->setPosition(lightPos);

        // Rotate cube
        Matrix3 rot = Matrix3::fromXRotation(angleChange);
        rot.multiply(Matrix3::fromZRotation(angleChange), &rot);
        colorCube->addRotation(rot);

        renderer->render(scene, camera);
    }
}
