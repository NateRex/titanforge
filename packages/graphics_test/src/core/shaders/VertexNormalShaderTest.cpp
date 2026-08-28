#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/VertexNormalShader.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/cameras/PerspectiveCamera.h>

/**
 * Tests the ability to set a render item.
 */
BOOST_AUTO_TEST_CASE(VertexNormalShader_setItem)
{
    GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
    MaterialPtr material = MeshMaterial::create();

    DrawItem item;
    item.geometry = geometry.get();
    item.material = material.get();

    ShaderPtr shader = ShaderManager::getShader(ShaderId::VERTEX_NORMALS);
    BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a camera.
 */
BOOST_AUTO_TEST_CASE(VertexNormalShader_setCamera)
{
    CameraPtr camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);
    ShaderPtr shader = ShaderManager::getShader(ShaderId::VERTEX_NORMALS);
    BOOST_REQUIRE_NO_THROW(shader->setCamera(camera.get()));
}
