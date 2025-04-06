#include <graphics/Engine.h>
#include <graphics/core/windows/WindowManager.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <graphics/core/Buffer.h>
#include <graphics/textures/Texture.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/entities/Mesh.h>
#include <graphics/materials/BasicMaterial.h>
#include <math/Vector3.h>
#include <math/Matrix4.h>
#include <common/Utils.h>

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
    window->setBackgroundColor(Color(0.2f, 0.3f, 0.3f, 1.0f));

    // Geometry
    GeometryPtr geometry = BoxGeometry::create(1, 1, 1);

    // Material
    TexturePtr texture = TextureLoader::getInstance().load("assets/container.jpg");
    MaterialPtr material = BasicMaterial::Builder()
        .setTexture(texture)
        .build();

    // Shaders
    ShaderPtr shader = ShaderManager::getInstance().getShader(MaterialType::BASIC);
    shader->setMaterial(material);
    shader->setModelMatrix(getModelMatrix());
    shader->setViewMatrix(getViewMatrix());
    shader->setProjectionMatrix(getProjectionMatrix());

    // Render loop
    while (window->isOpen())
    {
        Engine::startFrame();

        shader->setModelMatrix(getModelMatrix());
        shader->draw(geometry->getBuffer());

        Engine::finishFrame();
    }

    Engine::stop();
}
