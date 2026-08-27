#include <boost/test/unit_test.hpp>
#include <graphics/objects/Mesh.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/core/renderer/RenderPass.h>
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
 * Tests that a mesh is applied to the state when in material rendering mode
 */
BOOST_AUTO_TEST_CASE(Mesh_materialModeTraversal)
{
	const MeshPtr mesh = Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create());
	RenderPass pass;
	pass.mode = RenderMode::MATERIAL;

	RenderState state;
	mesh->traverse(state, pass, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(state.items.size() == 1);
	BOOST_TEST(state.items[0].shader == ShaderId::MESH);
}

/**
 * Tests that a mesh is applied to the state when in vertex normal rendering mode
 */
BOOST_AUTO_TEST_CASE(Mesh_vertexNormalModeTraversal)
{
	const MeshPtr mesh = Mesh::create(BoxGeometry::create(1.f, 1.f, 1.f), MeshMaterial::create());
	RenderPass pass;
	pass.mode = RenderMode::VERTEX_NORMALS;

	RenderState state;
	mesh->traverse(state, pass, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(state.items.size() == 1);
	BOOST_TEST(state.items[0].shader == ShaderId::VERTEX_NORMALS);
}