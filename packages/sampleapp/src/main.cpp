#include <graphics/core/Renderer.h>
#include <graphics/scene/Scene.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/entities/Mesh.h>
#include <graphics/core/windows/Window.h>
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
 * Main entrypoint for the application
 */
int main()
{
    // Create renderer
    Renderer renderer;
    renderer.setBackgroundColor(Color(0.2f, 0.3f, 0.3f, 1.0f));

    // Create scene
    ScenePtr scene = Scene::create();
    std::vector<MeshPtr> meshes = createBoxes(scene);

    // Render the scene until the window is closed, rotating the meshes on each animation frame.
    while (renderer.getWindow()->isOpen())
    {
        int idx = 0;
        for (MeshPtr mesh : meshes)
        {
            Matrix3 rotation = Matrix3::fromRotation(Vector3(1.f, 0.3f, 0.5f), renderer.getTime() * deg2Rad(20.f * idx));
            mesh->setRotation(rotation);
            idx++;
        }

        renderer.render(scene);
    }
}
