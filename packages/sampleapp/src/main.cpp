#include <graphics/core/Renderer.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/entities/Mesh.h>
#include <graphics/core/windows/Window.h>
#include <common/Utils.h>
#include <cmath>

/**
 * Main entrypoint for the application
 */
int main()
{
    Renderer renderer;
    renderer.setBackgroundColor(Color(0.2f, 0.3f, 0.3f, 1.0f));

    GeometryPtr geometry = BoxGeometry::create(1, 1, 1);
    
    MaterialPtr material = BasicMaterial::create();
    material->texture = TextureLoader::load("assets/container.jpg");

    MeshPtr mesh = Mesh::create(geometry, material);

    while (renderer.getWindow()->isOpen())
    {
        Matrix3 rotation = Matrix3::fromRotation(Vector3(0.5f, 1.0f, 0.f), renderer.getTime() * deg2Rad(50.f));
        mesh->setRotation(rotation);

        renderer.render(mesh);
    }
}
