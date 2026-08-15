#include <boost/test/unit_test.hpp>
#include <graphics/loaders/ModelLoader.h>
#include <graphics/core/EntityGroup.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <graphics/objects/Mesh.h>

BOOST_AUTO_TEST_SUITE(ModelLoaderTest)

namespace
{
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
}

BOOST_AUTO_TEST_CASE(loads_model_as_entity_hierarchy)
{
	EntityGroupPtr model = ModelLoader::load("assets/triangle.obj");
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

BOOST_AUTO_TEST_CASE(throws_when_model_cannot_be_loaded)
{
	BOOST_CHECK_THROW(ModelLoader::load("assets/missing.obj"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
