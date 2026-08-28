#include <boost/test/unit_test.hpp>
#include <graphics/objects/Mesh.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/core/renderer/DrawState.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic construction of a mesh
 */
BOOST_AUTO_TEST_CASE(Mesh_basics)
{
	GeometryPtr geometry = BoxGeometry::create(1, 1, 1);
	MeshMaterialPtr material = MeshMaterial::create();
	MeshPtr mesh = Mesh::create(geometry, material);

	BOOST_TEST(mesh->geometry == geometry);
	BOOST_TEST(mesh->material == material);
}

/**
 * Tests that a mesh is applied to the state during scene traversal
 */
BOOST_AUTO_TEST_CASE(Mesh_traversal)
{
	const MeshPtr mesh = Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create());

	DrawState state;
	mesh->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(state.items.size() == 1);
	BOOST_TEST(state.items[0].variants.size() == 2);
    BOOST_TEST(state.items[0].variants[0].mode == RenderModes::MATERIAL);
	BOOST_TEST(state.items[0].variants[1].mode == RenderModes::VERTEX_NORMALS);
}
