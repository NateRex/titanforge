#include <boost/test/unit_test.hpp>
#include <graphics/loaders/ModelLoader.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <graphics/objects/Mesh.h>
#include <common/Utils.h>
#include <common/exceptions/InstantiationException.h>

MeshPtr findMesh(const EntityPtr& entity)
{
	if (entity->entityType == EntityType::MESH)
	{
		return std::static_pointer_cast<Mesh>(entity);
	}

	for (unsigned int i = 0; i < entity->getNumberOfChildren(); ++i)
	{
		MeshPtr mesh = findMesh(entity->getChild(i));
		if (mesh != nullptr)
		{
			return mesh;
		}
	}

	return nullptr;
}

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

BOOST_AUTO_TEST_CASE(ModelLoader_failureCase)
{
	BOOST_CHECK_THROW(ModelLoader::load("assets/missing.obj"), InstantiationException);
}
