#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/PostProcessShader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/loaders/TextureLoader.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests that a fully populated post-processing material can be applied
 */
BOOST_AUTO_TEST_CASE(PostProcessShader_setMaterial)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	material->texture = TextureLoader::load("assets/container.jpg");
	material->color = Color(0.25f, 0.5f, 0.75f, 0.8f);
	material->exposure = 1.5f;
	material->saturation = 0.6f;
	material->contrast = 1.2f;

	ShaderPtr shader = PostProcessShader::create();
	shader->activate();

	BOOST_REQUIRE_NO_THROW(shader->setMaterial(material.get()));
}

/**
 * Tests that a null material is rejected.
 */
BOOST_AUTO_TEST_CASE(PostProcessShader_setNullMaterial)
{
	ShaderPtr shader = PostProcessShader::create();
	BOOST_REQUIRE_THROW(shader->setMaterial(nullptr), IllegalArgumentException);
}

/**
 * Tests that a material intended for another shader is rejected.
 */
BOOST_AUTO_TEST_CASE(PostProcessShader_setWrongMaterialType)
{
	ShaderPtr shader = PostProcessShader::create();
	MeshMaterialPtr material = MeshMaterial::create();
	BOOST_REQUIRE_THROW(shader->setMaterial(material.get()), IllegalArgumentException);
}

/**
 * Tests that a post-processing material without an input texture is rejected.
 */
BOOST_AUTO_TEST_CASE(PostProcessShader_setMaterialWithoutTexture)
{
	ShaderPtr shader = PostProcessShader::create();
	PostProcessMaterialPtr material = PostProcessMaterial::create();

	BOOST_REQUIRE_THROW(shader->setMaterial(material.get()), IllegalArgumentException);
}
