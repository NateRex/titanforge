#include <graphics/Engine.h>
#include <graphics/windows/WindowManager.h>
#include <graphics/windows/Window.h>
#include <graphics/windows/InputController.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/buffers/BufferManager.h>
#include <graphics/textures/Texture.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/primitives/Primitive.h>
#include <geometry/Vector3.h>
#include <geometry/Matrix4.h>
#include <common/Utils.h>

/**
 * @return An example polyface
 */
Primitive examplePolyface()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    return Primitive(vertices, 180, { false, true });
}

/**
 * @return Model matrix
 */
Matrix4 getModelMatrix()
{
    return Matrix4::fromRotation(Vector3(0.5f, 1.0f, 0.f), Engine::getTime() * deg2Rad(50.f));
}

/**
 * @return View matrix
 */
Matrix4 getViewMatrix()
{
    return Matrix4::fromTranslation(Vector3(0.f, 0.f, -3.f));
}

/**
 * @return Projection matrix
 */
Matrix4 getProjectionMatrix()
{
    return Matrix4::fromPerspective(deg2Rad(45.f), 800.f / 600.f, 0.1f, 100.f);
}

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();

    // Configure window
    Window* window = WindowManager::getCurrent();
    window->setBackgroundColor(Color::fromFloats(0.2f, 0.3f, 0.3f, 1.0f));

    // Textures
    Texture* boxTexture = TextureManager::create("box", "assets/container.jpg");

    // Buffers
    Buffer* buffer = BufferManager::startBuffer("geometry")
        .add(examplePolyface())
        .finish();

    // Shaders
    Shader* shader = ShaderManager::get("tf_basic");
    shader->setUniform("tex", 0, boxTexture);
    shader->setUniform("model", getModelMatrix());
    shader->setUniform("view", getViewMatrix());
    shader->setUniform("proj", getProjectionMatrix());

    // Render loop
    while (window->isOpen())
    {
        Engine::startFrame();

        shader->setUniform("model", getModelMatrix());
        shader->draw(buffer);

        Engine::finishFrame();
    }

    Engine::stop();
}
