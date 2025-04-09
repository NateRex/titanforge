#include <boost/test/unit_test.hpp>
#include <graphics/entities/Mesh.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/BasicMaterial.h>

/**
 * Tests the basic construction of a mesh
 */
BOOST_AUTO_TEST_CASE(Mesh_basics)
{
	GeometryPtr geometry = BoxGeometry::create(1, 1, 1);
	MaterialPtr material = BasicMaterial::create();
	MeshPtr mesh = Mesh::create(geometry, material);

	BOOST_TEST(mesh->geometry == geometry);
	BOOST_TEST(mesh->material == material);
}