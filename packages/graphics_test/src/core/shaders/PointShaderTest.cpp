#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/PointShader.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/materials/PointMaterial.h>
#include <graphics/cameras/PerspectiveCamera.h>

/**
 * Tests the ability to set a render item. This, in turn, also invokes the method to assign material uniforms.
 */
BOOST_AUTO_TEST_CASE(PointShader_setItem)
{
    GeometryPtr geometry = PointGeometry::create(Vector3::ZERO);
    MaterialPtr material = PointMaterial::create();

	DrawItem item;
    item.geometry = geometry.get();
    item.material = material.get();

	ShaderPtr shader = ShaderManager::getShader(ShaderId::POINT);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a camera
 */
BOOST_AUTO_TEST_CASE(PointShader_setCamera)
{
    CameraPtr camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);
    ShaderPtr shader = ShaderManager::getShader(ShaderId::POINT);
	BOOST_REQUIRE_NO_THROW(shader->setCamera(camera.get()));
}
