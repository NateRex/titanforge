#include <graphics/core/Renderer.h>
#include <graphics/scene/Scene.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/entities/Mesh.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/modifiers/InputModifiers.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Utils.h>
#include <cmath>

/**
 * Creates several boxes and adds them to the given scene
 * @return A vector containing pointers to the meshes that were created
 */
std::vector<MeshPtr> createBoxes(ScenePtr scene)
{
    GeometryPtr geometry = BoxGeometry::create(1, 1, 1);
    MaterialPtr material = BasicMaterial::create();
    material->texture = TextureLoader::load("assets/container.jpg");

    float positions[] =
    {
        0.0f,  0.0f,  0.0f,
        2.0f,  5.0f, -15.0f,
        -1.5f, -2.2f, -2.5f,
        -3.8f, -2.0f, -12.3f,
        2.4f, -0.4f, -3.5f,
        -1.7f,  3.0f, -7.5f,
        1.3f, -2.0f, -2.5f,
        1.5f,  2.0f, -2.5f,
        1.5f,  0.2f, -1.5f,
        -1.3f,  1.0f, -1.5f
    };

    int numPositions = sizeof(positions) / sizeof(positions[0]);
    std::vector<MeshPtr> meshes;

    for (int i = 0; i < numPositions; i += 3)
    {
        MeshPtr mesh = Mesh::create(geometry, material);
        mesh->setPosition(positions[i], positions[i + 1], positions[i + 2]);
        scene->add(mesh);
        meshes.push_back(mesh);
    }

    return meshes;
}

/**
 * Creates a camera capable of being controlled via the 'a', 'w', 's', and 'd' keys
 * @param window The window context
 * @return The camera that was created
 */
CameraPtr createCamera(WindowPtr window)
{
    PerspectiveCameraPtr camera = PerspectiveCamera::create(45.f, 800.f / 600.f, 0.1f, 100.f);

    InputAction move("Move", InputValueType::VECTOR_2D);
    InputAction look("Look", InputValueType::VECTOR_2D);
    InputAction zoom("Zoom", InputValueType::SCALAR);

    // Create key bindings
    InputContextPtr context = InputContext::create();
    context->add(DigitalInput::KEY_D, InputTrigger::HELD, move);
    context->add(DigitalInput::KEY_A, InputTrigger::HELD, move, InputModifiers().negateX());
    context->add(DigitalInput::KEY_W, InputTrigger::HELD, move, InputModifiers().swizzle(Axis::Y, Axis::X, Axis::Z));
    context->add(DigitalInput::KEY_S, InputTrigger::HELD, move, InputModifiers().negateX().swizzle(Axis::Y, Axis::X, Axis::Z));
    context->add(AxisInput::MOUSE_MOVE, look, InputModifiers().negateY());
    context->add(AxisInput::MOUSE_SCROLL, zoom, InputModifiers().swizzle(Axis::Y, Axis::X, Axis::Z));

    InputController* inputController = window->getInputController();
    inputController->addContext(context);

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
    Renderer renderer;
    renderer.setBackgroundColor(Color(0.2f, 0.3f, 0.4f, 1.0f));

    CameraPtr camera = createCamera(renderer.getWindow());
    camera->lookAt(Vector3(0.f, 0.f, 10.f), Vector3::ZERO, Vector3::YHAT);

    ScenePtr scene = Scene::create();
    std::vector<MeshPtr> meshes = createBoxes(scene);

    while (renderer.getWindow()->isOpen())
    {
        float t = renderer.getTime();

        // Rotate meshes
        for (int i = 0; i < meshes.size(); i++)
        {
            MeshPtr mesh = meshes[i];
            mesh->addRotation(Matrix3::fromRotation(Vector3(0.5f, 1.0f, 0.0f), i * 0.001));
        }

        // Render scene
        renderer.render(scene, camera);
    }
}
