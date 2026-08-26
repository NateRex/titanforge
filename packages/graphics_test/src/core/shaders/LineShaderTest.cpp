#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/LineShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/geometry/LineGeometry.h>
#include <graphics/materials/LineMaterial.h>
#include <graphics/cameras/PerspectiveCamera.h>

/**
 * Tests the ability to set a render item. This, in turn, also invokes the method to assign material uniforms.
 */
BOOST_AUTO_TEST_CASE(LineShader_setItem)
{
    std::vector<Vector3> segments = { Vector3::MINUS_XHAT, Vector3::XHAT };
    GeometryPtr geometry = LineGeometry::createSegments(segments.data(), segments.size());
    MaterialPtr material = LineMaterial::create();

	RenderItem item;
    item.geometry = geometry.get();
    item.material = material.get();

	ShaderPtr shader = ShaderManager::getShader(MaterialType::LINE);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a camera
 */
BOOST_AUTO_TEST_CASE(LineShader_setCamera)
{
    CameraPtr camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);
    ShaderPtr shader = ShaderManager::getShader(MaterialType::LINE);
	BOOST_REQUIRE_NO_THROW(shader->setCamera(camera.get()));
}