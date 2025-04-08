#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/BasicShader.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/TextureLoader.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests that the model, view, and projection matrix uniforms can be set in the basic shader
 */
BOOST_AUTO_TEST_CASE(BasicShader_matrices)
{
	ShaderPtr shader = ShaderManager::getInstance().getShader(MaterialType::BASIC);

	Matrix4 m;
	BOOST_REQUIRE_NO_THROW(shader->setModelMatrix(m));
	BOOST_REQUIRE_NO_THROW(shader->setViewMatrix(m));
	BOOST_REQUIRE_NO_THROW(shader->setProjectionMatrix(m));
}

/**
 * Tests that the material uniforms can be set in the basic shader
 */
BOOST_AUTO_TEST_CASE(BasicShader_material)
{
	ShaderPtr shader = ShaderManager::getInstance().getShader(MaterialType::BASIC);

	// Without texture
	MaterialPtr material = BasicMaterial::create();
	BOOST_REQUIRE_NO_THROW(shader->setMaterial(material));

	// With texture
	material->texture = TextureLoader::getInstance().load("assets/container.jpg");
	BOOST_REQUIRE_NO_THROW(shader->setMaterial(material));
}

/**
 * Tests that an exception is thrown when trying to apply the wrong material
 */
BOOST_AUTO_TEST_CASE(BasicShader_wrongMaterial)
{
	// Define material that cannot be cast to a BasicMaterial
	// TODO (Nate) - Remove this once there actually exists more than one type of material
	class TestMaterial : public Material
	{
	public:
		TestMaterial() : Material(MaterialType::BASIC) {}
	};

	ShaderPtr shader = ShaderManager::getInstance().getShader(MaterialType::BASIC);
	MaterialPtr material = std::shared_ptr<TestMaterial>(new TestMaterial());
	BOOST_REQUIRE_THROW(shader->setMaterial(material), IllegalArgumentException);
}