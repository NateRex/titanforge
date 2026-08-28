#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/MeshShader.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/loaders/TextureLoader.h>
#include <graphics/lights/AmbientLight.h>
#include <graphics/lights/PointLight.h>
#include <graphics/lights/DirectionalLight.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/objects/Skybox.h>
#include <graphics/geometry/BoxGeometry.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests the ability to set a fully-populated the render state
 */
BOOST_AUTO_TEST_CASE(MeshShader_setState)
{
	DrawState state;
	LightPtr ambient = AmbientLight::create();
	LightPtr point = PointLight::create();
	LightPtr directional = DirectionalLight::create();
	LightPtr spot = SpotLight::create();
	state.lights.push_back({ambient.get()});
	state.lights.push_back({point.get()});
	state.lights.push_back({directional.get()});
	state.lights.push_back({spot.get()});

	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests the ability to set a render state containing no lighting
 */
BOOST_AUTO_TEST_CASE(MeshShader_setStateNoLighting)
{
	DrawState state;
	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests the ability to set a render state containing environment texture
 */
BOOST_AUTO_TEST_CASE(MeshShader_setEnvironment)
{
	DrawState state;
	TextureCubePtr environment = Skybox::createDefaultTexture();
	state.environment.texture = environment.get();
	state.environment.intensity = 1.5f;
	state.environment.rotation = 0.25f;

	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
	BOOST_REQUIRE_NO_THROW(shader->setState(state));
}

/**
 * Tests that light intensity is an open-ended multiplier rather than a value
 * limited to the [0, 1] range.
 */
BOOST_AUTO_TEST_CASE(MeshShader_setStateHighIntensity)
{
	DrawState state;

	LightPtr light = PointLight::create();
	light->intensity = 4.f;
	state.lights.push_back({light.get()});

	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
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

	DrawItem item;
	item.geometry = geom.get();
	item.material = mat.get();
	item.modelTransform = Matrix4::IDENTITY;
	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
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

	DrawItem item;
	item.geometry = geom.get();
	item.material = mat.get();
	item.modelTransform = Matrix4::IDENTITY;
	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
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

 	DrawItem item;
	item.geometry = geom.get();
	item.material = mat.get();
 	item.modelTransform = Matrix4::IDENTITY;
 	item.normalTransform = Matrix3::IDENTITY;

	ShaderPtr shader = ShaderManager::getShader(ShaderId::MESH);
 	BOOST_REQUIRE_NO_THROW(shader->setItem(item));
 }
