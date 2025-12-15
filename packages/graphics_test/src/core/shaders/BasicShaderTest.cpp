#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/BasicShader.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/lights/Light.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests that the model, view, and projection matrix uniforms can be set
 */
BOOST_AUTO_TEST_CASE(BasicShader_matrices)
{
	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);

	Matrix4 m;
	BOOST_REQUIRE_NO_THROW(shader->setModelMatrix(m));
	BOOST_REQUIRE_NO_THROW(shader->setViewMatrix(m));
	BOOST_REQUIRE_NO_THROW(shader->setProjectionMatrix(m));
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
