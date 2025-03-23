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
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <geometry/Matrix3.h>
#include <geometry/Matrix4.h>
#include <common/Utils.h>

/**
 * @return An example polyface
 */
PPolyface examplePolyface()
{
    Vector3 vertices[] = {
        Vector3(-0.5f, -0.5f, -0.5f),   // 0: far bottom left
        Vector3(0.5f, -0.5f, -0.5f),    // 1: far bottom right
        Vector3(0.5f, 0.5f, -0.5f),     // 2: far top right
        Vector3(-0.5f, 0.5f, -0.5f),    // 3: far top left
        Vector3(-0.5f, -0.5f, 0.5f),    // 4: near bottom left
        Vector3(0.5f, -0.5f, 0.5f),     // 5: near bottom right
        Vector3(0.5f, 0.5f, 0.5f),      // 6: near top right
        Vector3(-0.5f, 0.5f, 0.5f),     // 7: near top left
    };
    Vector2 texCoords[] = {
        Vector2(0, 0),
        Vector2(1, 0),
        Vector2(1, 1),
        Vector2(0, 1),
        Vector2(0, 1),
        Vector2()
    };
    int indices[] = {
        // Back face
        0, 1, 2, -1,
        2, 3, 0, -1,
        
        // Left face
        0, 4, 7, -1,
        7, 3, 0, -1,

        // Front face
        4, 5, 6, -1,
        6, 7, 4, -1,

        // Right face
        1, 5, 6, -1,
        6, 2, 0, -1,

        // Top face
        7, 3, 2, -1,
        2, 6, 7, -1,

        // Bottom face
        4, 5, 1, -1,
        1, 0, 4, -1
    };
    return PPolyface(vertices, 8, indices, 48, nullptr, texCoords);
}

/**
 * @return Model matrix
 */
Matrix4 getModelMatrix()
{
    return Matrix4::fromXRotation(deg2Rad(-55.f));
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

        shader->draw(buffer);

        Engine::finishFrame();
    }

    Engine::stop();
}
