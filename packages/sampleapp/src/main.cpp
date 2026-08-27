#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/scene/Scene.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/materials/PointMaterial.h>
#include <graphics/materials/LineMaterial.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/objects/Points.h>
#include <graphics/objects/Lines.h>
#include <graphics/objects/Skybox.h>
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
CameraPtr setupInputs(RendererPtr renderer)
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
    inputController->bind(quit, [window = window.get()](InputValue value, float deltaTime)
    {
        window->close();
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
    CameraPtr camera = setupInputs(renderer);
    camera->lookAt(Vector3(0.f, 0.f, 10.f), Vector3::ZERO, Vector3::YHAT);

    ScenePtr scene = Scene::create();

    // Create skybox
    scene->add(Skybox::create());

    // Create point primitives
    PointMaterialPtr pointMat = PointMaterial::create();
    pointMat->color = Color::GREEN;
    pointMat->size = 0.09f;
    pointMat->sizeUnits = PrimitiveSizeUnits::WORLD;
    PointsPtr points = Points::create({
        Vector3(0.f, 5.f, 0.f),
        Vector3(2.f, 6.f, 0.f)
    }, pointMat);
    scene->add(points);

    // Create line primitives
    LineMaterialPtr lineMat = LineMaterial::create();
    lineMat->color = Color::RED;
    lineMat->width = 0.05f;
    lineMat->widthUnits = PrimitiveSizeUnits::WORLD;
    LinesPtr lines = Lines::createPolyline({
        Vector3(-7.f, 0.f, 0.f),
        Vector3(-4.f, 0.f, 0.f),
        Vector3(-4.f, 2.f, 0.f),
        Vector3(-7.f, 2.f, 0.f)
    }, lineMat, true);
    scene->add(lines);

    // Create glass cube
    MeshMaterialPtr boxMat = MeshMaterial::create();
    boxMat->refraction = 1.f;
    MeshPtr box = Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), boxMat);
    box->setPosition(2.f, 2.f, 2.f);
    scene->add(box);

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

    // Create target for offscreen rendering
    RenderTargetPtr target = RenderTarget::create();

    float rotationRate = 0.25f;
    while (renderer->getWindow()->isOpen())
    {
        // Rotate entity
        entity->addRotation(Matrix3::fromYRotation(rotationRate * renderer->getDeltaTime()));

        // Render scene to offscreen target
        RenderPass pass;
        pass.target = target;
        pass.clearFlags = ClearFlags::COLOR | ClearFlags::DEPTH;
        renderer->renderPass(scene, camera, pass);

        // Render normals
        pass.mode = RenderMode::SURFACE_NORMALS;
        pass.clearFlags = ClearFlags::NONE;
        renderer->renderPass(scene, camera, pass);

        // Post-processing effects
        postProcessing->texture = target->colorTexture(0);
        pass.target = nullptr;
        pass.mode = RenderMode::MATERIAL;
        pass.clearFlags = ClearFlags::COLOR;
        renderer->renderPass(postProcessing, pass);

		// Present the frame after all passes are complete
		renderer->present();
    }

    renderer->destroy();
}
