#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/core/shaders/SkyboxShader.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests that a camera can be applied
 */
BOOST_AUTO_TEST_CASE(MeshShader_setCamera)
{
	CameraPtr camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);

	ShaderPtr shader = SkyboxShader::create();
	BOOST_REQUIRE_NO_THROW(shader->setCamera(camera.get()));
}

/**
 * Tests that a fully-populated skybox material can be applied
 */
BOOST_AUTO_TEST_CASE(SkyboxShader_setMaterial)
{
	SkyboxMaterialPtr material = SkyboxMaterial::create();
	material->texture = TextureCube::create(TextureCubeConfig(), {});

	ShaderPtr shader = SkyboxShader::create();
	shader->activate();

	BOOST_REQUIRE_NO_THROW(shader->setMaterial(material.get()));
}

/**
 * Tests that a null camera is rejected.
 */
BOOST_AUTO_TEST_CASE(SkyboxShader_setNullCamera)
{
	ShaderPtr shader = SkyboxShader::create();
	BOOST_REQUIRE_THROW(shader->setCamera(nullptr), IllegalArgumentException);
}

/**
 * Tests that a null material is rejected.
 */
BOOST_AUTO_TEST_CASE(SkyboxShader_setNullMaterial)
{
	ShaderPtr shader = SkyboxShader::create();
	BOOST_REQUIRE_THROW(shader->setMaterial(nullptr), IllegalArgumentException);
}

/**
 * Tests that a material intended for another shader is rejected.
 */
BOOST_AUTO_TEST_CASE(SkyboxShader_setWrongMaterialType)
{
	ShaderPtr shader = SkyboxShader::create();
	MeshMaterialPtr material = MeshMaterial::create();
	BOOST_REQUIRE_THROW(shader->setMaterial(material.get()), IllegalArgumentException);
}

/**
 * Tests that a post-processing material without an input texture is rejected.
 */
BOOST_AUTO_TEST_CASE(SkyboxShader_setMaterialWithoutTexture)
{
	ShaderPtr shader = SkyboxShader::create();
	SkyboxMaterialPtr material = SkyboxMaterial::create();
	BOOST_REQUIRE_THROW(shader->setMaterial(material.get()), IllegalArgumentException);
}
