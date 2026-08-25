#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/MeshShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/loaders/TextureLoader.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/PointLight.h>
#include <graphics/lights/DirectionalLight.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/objects/Mesh.h>
#include <graphics/objects/Skybox.h>
#include <graphics/geometry/BoxGeometry.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests the ability to set a fully-populated the render state
 */
BOOST_AUTO_TEST_CASE(MeshShader_setState)
{
	RenderState state;
	state.lighting.lights.push_back({AmbientLight::create()});
	state.lighting.lights.push_back({PointLight::create()});
	state.lighting.lights.push_back({DirectionalLight::create()});
	state.lighting.lights.push_back({SpotLight::create()});
	state.camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);

	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests the ability to set a render state containing no lighting
 */
BOOST_AUTO_TEST_CASE(MeshShader_setStateNoLighting)
{
	RenderState state;
	state.camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);

	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests the ability to set a render state containing environment texture
 */
BOOST_AUTO_TEST_CASE(MeshShader_setEnvironment)
{
	RenderState state;
	state.camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);
	state.environment.texture = Skybox::createDefaultTexture();
	state.environment.intensity = 1.5f;
	state.environment.rotation = 0.25f;

	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests that light intensity is an open-ended multiplier rather than a value
 * limited to the [0, 1] range.
 */
BOOST_AUTO_TEST_CASE(MeshShader_setStateHighIntensity)
{
	RenderState state;
	state.camera = PerspectiveCamera::create(60.f, 800.f / 600.f, 0.1f, 100.f);

	LightPtr light = PointLight::create();
	light->intensity = 4.f;
	state.lighting.lights.push_back({light});

	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
	shader->activate();
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests the ability to set the render item consisting of a basic colored material
 */
BOOST_AUTO_TEST_CASE(MeshShader_setColorMaterial)
{
	GeometryPtr geom = BoxGeometry::create(1.f, 1.f, 1.f);
	MeshMaterialPtr mat = MeshMaterial::create();
	MeshPtr mesh = Mesh::create(geom, mat);

	RenderItem item;
	item.mesh = mesh;
	item.modelTransform = Matrix4::IDENTITY;
	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set the render item consisting of a material containing texture
 */
BOOST_AUTO_TEST_CASE(MeshShader_setTextureMaterial)
{
	GeometryPtr geom = BoxGeometry::create(1.f, 1.f, 1.f);
	MeshMaterialPtr mat = MeshMaterial::create();
	mat->texture = TextureLoader::load("assets/container.jpg");
	MeshPtr mesh = Mesh::create(geom, mat);

	RenderItem item;
	item.mesh = mesh;
	item.modelTransform = Matrix4::IDENTITY;
	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a render item consisting of a material containing diffuse and specular maps
 */
 BOOST_AUTO_TEST_CASE(MeshShader_setDiffuseMapMaterial)
 {
 	GeometryPtr geom = BoxGeometry::create(1.f, 1.f, 1.f);
 	MeshMaterialPtr mat = MeshMaterial::create();
	TexturePtr img = TextureLoader::load("assets/container.jpg");
 	mat->diffuseMap = img;
	mat->specularMap = img;
 	MeshPtr mesh = Mesh::create(geom, mat);

 	RenderItem item;
 	item.mesh = mesh;
 	item.modelTransform = Matrix4::IDENTITY;
 	item.normalTransform = Matrix3::IDENTITY;

 	ShaderPtr shader = ShaderManager::getShader(MaterialType::MESH);
 	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
 }
