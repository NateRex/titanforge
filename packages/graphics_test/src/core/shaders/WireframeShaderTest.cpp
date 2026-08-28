#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/materials/WireframeMaterial.h>
#include <graphics/cameras/PerspectiveCamera.h>

/**
 * Tests the ability to set a render item. This, in turn, also invokes the method to assign material uniforms.
 */
BOOST_AUTO_TEST_CASE(WireframeShader_setItem)
{
    std::vector<Vector3> segments = { Vector3::MINUS_XHAT, Vector3::XHAT };
    MaterialPtr material = WireframeMaterial::create();

	DrawItem item;
    item.material = material.get();

	ShaderPtr shader = ShaderManager::getShader(ShaderId::WIREFRAME);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a camera
 */
BOOST_AUTO_TEST_CASE(WireframeShader_setCamera)
{
    CameraPtr camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);
    ShaderPtr shader = ShaderManager::getShader(ShaderId::WIREFRAME);
	BOOST_REQUIRE_NO_THROW(shader->setCamera(camera.get()));
}
