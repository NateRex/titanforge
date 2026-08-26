#include <boost/test/unit_test.hpp>
#include <graphics/objects/Mesh.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
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