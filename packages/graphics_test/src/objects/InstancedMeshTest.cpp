#include <boost/test/unit_test.hpp>
#include <graphics/objects/InstancedMesh.h>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/PrintHelpers.h>

/**
 * Tests the wrapper around the transform data for an instance
 */
BOOST_AUTO_TEST_CASE(InstanceTransform_matrix)
{
	InstanceTransform instance;
	instance.position = Vector3(1.f, 2.f, 3.f);
	instance.scaling = Vector3(2.f, 3.f, 4.f);

	const Vector3 transformed = instance.matrix().transformPosition(Vector3(1.f, 1.f, 1.f));
	BOOST_TEST(transformed.x == 3.f);
	BOOST_TEST(transformed.y == 5.f);
	BOOST_TEST(transformed.z == 7.f);
}

/**
 * Tests out basic creation and sizing of an instanced mesh
 */
BOOST_AUTO_TEST_CASE(InstancedMesh_basics)
{
	GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
	MaterialPtr material = MeshMaterial::create();
	InstancedMeshPtr instances = InstancedMesh::create(geometry, material);

	BOOST_TEST(instances->geometry == geometry);
	BOOST_TEST(instances->material == material);
	BOOST_TEST(instances->size() == 0);

	instances = InstancedMesh::create(geometry, material, { InstanceTransform(), InstanceTransform() });
	BOOST_TEST(instances->size() == 2);
}

/**
 * Tests the ability to get, set, and replace instances of a mesh
 */
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

	InstanceTransform updated;
	updated.scaling = Vector3(2.f, 3.f, 4.f);
	mesh->setInstance(1, updated);
	BOOST_TEST(mesh->getInstance(1).scaling.y == 3.f);

	mesh->setInstances({ updated });
	BOOST_TEST(mesh->size() == 1);
	BOOST_TEST(mesh->getInstance(0).scaling.z == 4.f);

	mesh->removeInstance(0);
	mesh->addInstance();
	mesh->clearInstances();
	BOOST_TEST(mesh->size() == 0);
}

/**
 * Tests exceptions are thrown when accessing out-of-range instance indices
 */
BOOST_AUTO_TEST_CASE(InstancedMesh_invalidIndices)
{
	InstancedMeshPtr mesh = InstancedMesh::create(BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create());
	BOOST_CHECK_THROW(mesh->getInstance(0), IllegalArgumentException);
	BOOST_CHECK_THROW(mesh->removeInstance(0), IllegalArgumentException);
	BOOST_CHECK_THROW(mesh->setInstance(0, InstanceTransform()), IllegalArgumentException);
}

/**
 * Tests that traversal correctly applies an instanced mesh to a rendering state
 */
BOOST_AUTO_TEST_CASE(InstancedMesh_traversal)
{
	InstanceTransform instance;
	instance.scaling = Vector3(2.f, 3.f, 4.f);
	InstancedMeshPtr mesh = InstancedMesh::create(
		BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create(), { instance });

	DrawState state;
	mesh->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(state.items.size() == 1);

	const DrawItem& item = state.items[0];
	BOOST_TEST(item.geometryBuffer == nullptr);
	BOOST_REQUIRE(item.instanceBuffer != nullptr);
	BOOST_TEST(item.instanceBuffer->getNumberOfInstances() == 1);
	BOOST_TEST(item.material == mesh->material.get());
	BOOST_TEST(item.variants.size() == 1);
	BOOST_TEST(item.variants[0].mode == RenderModes::MATERIAL);
	BOOST_TEST(item.variants[0].shader == ShaderId::INSTANCED_MESH);
	BOOST_TEST(item.variants[0].layer == DrawLayer::OPAQUE);
}

/**
 * Tests that the buffer is updated during scene traversal
 */
BOOST_AUTO_TEST_CASE(InstancedMesh_updatesBuffer)
{
	InstancedMeshPtr mesh = InstancedMesh::create(
		BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create(), { InstanceTransform() });

	// On first traversal, buffer should be created. Will contain a single instance.
	DrawState firstState;
	mesh->traverse(firstState, Matrix4::IDENTITY, Matrix3::IDENTITY);
	InstanceBuffer* buffer = firstState.items[0].instanceBuffer;
	BOOST_REQUIRE(buffer != nullptr);
	BOOST_TEST(buffer->getNumberOfInstances() == 1);

	mesh->addInstance();

	// On second traversal, buffer pointer should be unchanged, however the buffer was updated
	// to contain two instances
	DrawState secondState;
	mesh->traverse(secondState, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(secondState.items[0].instanceBuffer == buffer);
	BOOST_TEST(buffer->getNumberOfInstances() == 2);
}
