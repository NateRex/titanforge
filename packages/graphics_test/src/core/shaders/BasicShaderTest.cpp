#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/BasicShader.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/lights/Light.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests that the model, view, and projection matrix uniforms can be set
 */
BOOST_AUTO_TEST_CASE(BasicShader_matrices)
{
	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);

	Matrix4 m1;
	BOOST_REQUIRE_NO_THROW(shader->setModelMatrix(m1));

	Matrix3 m2;
	BOOST_REQUIRE_NO_THROW(shader->setNormalMatrix(m2));
}

/**
 * Tests that ambient lighting can be set
 */
BOOST_AUTO_TEST_CASE(BasicShader_ambientLighting)
{
	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);

	LightPtr light = Light::create();
	BOOST_REQUIRE_NO_THROW(shader->setAmbientLighting(light));
}

/**
 * Tests that a positional light can be set
 */
BOOST_AUTO_TEST_CASE(BasicShader_positionalLight)
{
	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);

	LightPtr light = Light::create();
	BOOST_REQUIRE_NO_THROW(shader->setPositionalLight(light));
}

/**
 * Tests that a camera can be set
 */
BOOST_AUTO_TEST_CASE(BasicShader_camera)
{
	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);

	CameraPtr camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);
	BOOST_REQUIRE_NO_THROW(shader->setCamera(camera));
}

/**
 * Tests that the material uniforms can be set
 */
BOOST_AUTO_TEST_CASE(BasicShader_material)
{
	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);

	// Without texture
	MaterialPtr material = BasicMaterial::create();
	BOOST_REQUIRE_NO_THROW(shader->setMaterial(material));

	// With texture
	material->texture = TextureLoader::load("assets/container.jpg");
	BOOST_REQUIRE_NO_THROW(shader->setMaterial(material));
}
