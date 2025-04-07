#include <graphics/core/Renderer.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/entities/Mesh.h>
#include <graphics/core/windows/Window.h>

/**
 * @return Model matrix
 */
//Matrix4 getModelMatrix()
//{
//    return Matrix4::fromRotation(Vector3(0.5f, 1.0f, 0.f), Engine::getTime() * deg2Rad(50.f));
//}

/**
 * Main entrypoint for the application
 */
int main() {
    Renderer renderer;

    // Geometry
    GeometryPtr geometry = BoxGeometry::create(1, 1, 1);

    // Material
    TexturePtr texture = TextureLoader::getInstance().load("assets/container.jpg");
    MaterialPtr material = BasicMaterial::Builder()
        .setTexture(texture)
        .build();

    // Entity
    MeshPtr mesh = Mesh::create(geometry, material);

    // Render loop
    while (renderer.getWindow()->isOpen())
    {
        renderer.render(mesh);
    }
}
