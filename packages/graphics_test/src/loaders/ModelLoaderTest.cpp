#include <boost/test/unit_test.hpp>
#include <graphics/loaders/ModelLoader.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/objects/Mesh.h>
#include <common/Utils.h>
#include <common/exceptions/InstantiationException.h>

MeshPtr findMesh(const EntityPtr& entity)
{
	MeshPtr mesh = std::dynamic_pointer_cast<Mesh>(entity);
	if (mesh)
	{
		return mesh;
	}


	for (unsigned int i = 0; i < entity->getNumberOfChildren(); ++i)
	{
		mesh = findMesh(entity->getChild(i));
		if (mesh != nullptr)
		{
			return mesh;
		}
	}

	return nullptr;
}

/**
 * Tests loading a model hierarchy with mesh geometry, material, normals, and texture coordinates
 */
BOOST_AUTO_TEST_CASE(ModelLoader_loadsHierarchy)
{
	EntityPtr model = ModelLoader::load("assets/triangle.obj");
	MeshPtr mesh = findMesh(model);

	BOOST_REQUIRE(model != nullptr);
	BOOST_REQUIRE(mesh != nullptr);
	BOOST_REQUIRE(mesh->getParent() != nullptr);
	BOOST_REQUIRE(mesh->geometry != nullptr);
	BOOST_REQUIRE(mesh->material != nullptr);
	BOOST_CHECK_EQUAL(mesh->geometry->size(), 3);

	GeometryAttributes attributes = mesh->geometry->getAttributes();
	BOOST_CHECK(attributes.normals);
	BOOST_CHECK(attributes.uvs);
}

/**
 * Tests that loading a missing model throws an instantiation exception
 */
BOOST_AUTO_TEST_CASE(ModelLoader_failureCase)
{
	BOOST_CHECK_THROW(ModelLoader::load("assets/missing.obj"), InstantiationException);
}
