#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/BasicShader.h>
#include <graphics/core/RenderState.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/PointLight.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/objects/Mesh.h>
#include <graphics/geometry/BoxGeometry.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests the ability to set the render state
 */
BOOST_AUTO_TEST_CASE(BasicShader_setState)
{
	RenderState state;
	state.lighting.ambient = AmbientLight::create();
	state.lighting.positional = PointLight::create();
	state.camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);

	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests the ability to set the render item consisting of a basic colored material
 */
BOOST_AUTO_TEST_CASE(BasicShader_setColorMaterial)
{
	GeometryPtr geom = BoxGeometry::create(1.f, 1.f, 1.f);
	MaterialPtr mat = BasicMaterial::create();
	MeshPtr mesh = Mesh::create(geom, mat);

	RenderItem item;
	item.mesh = mesh;
	item.modelTransform = Matrix4::IDENTITY;
	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set the render item consisting of a material containing texture
 */
BOOST_AUTO_TEST_CASE(BasicShader_setTextureMaterial)
{
	GeometryPtr geom = BoxGeometry::create(1.f, 1.f, 1.f);
	MaterialPtr mat = BasicMaterial::create();
	mat->texture = TextureLoader::load("assets/container.jpg");
	MeshPtr mesh = Mesh::create(geom, mat);

	RenderItem item;
	item.mesh = mesh;
	item.modelTransform = Matrix4::IDENTITY;
	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(MaterialType::BASIC);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}