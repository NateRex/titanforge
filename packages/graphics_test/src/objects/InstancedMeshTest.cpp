#include <boost/test/unit_test.hpp>
#include <graphics/objects/InstancedMesh.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/core/renderer/DrawState.h>

BOOST_AUTO_TEST_CASE(InstancedMesh_instances)
{
	InstancedMeshPtr mesh = InstancedMesh::create(BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create());
	InstanceTransform first;
	first.position = Vector3(1.f, 2.f, 3.f);
	mesh->addInstance(first);
	mesh->addInstance();

	BOOST_TEST(mesh->size() == 2);
	BOOST_TEST(mesh->getInstance(0).position.x == 1.f);
	BOOST_TEST(mesh->getInstance(0).position.y == 2.f);
	BOOST_TEST(mesh->getInstance(0).position.z == 3.f);
	mesh->removeInstance(1);
	BOOST_TEST(mesh->size() == 1);
	mesh->clearInstances();
	BOOST_TEST(mesh->size() == 0);
}

BOOST_AUTO_TEST_CASE(InstancedMesh_traversal)
{
	InstanceTransform instance;
	instance.scaling = Vector3(2.f, 3.f, 4.f);
	InstancedMeshPtr mesh = InstancedMesh::create(
		BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create(), { instance });

	DrawState state;
	mesh->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);

	BOOST_TEST(state.items.size() == 1);
	BOOST_TEST(state.items[0].instanceBuffer != nullptr);
	BOOST_TEST(state.items[0].instanceCount == 1);
	BOOST_TEST(state.items[0].variants.size() == 1);
	BOOST_TEST(static_cast<int>(state.items[0].variants[0].shader) == static_cast<int>(ShaderId::INSTANCED_MESH));
}
