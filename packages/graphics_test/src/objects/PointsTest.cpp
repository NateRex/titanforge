#include <boost/test/unit_test.hpp>
#include <graphics/objects/Points.h>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/materials/PointMaterial.h>
#include <graphics/core/renderer/DrawState.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic construction of points
 */
BOOST_AUTO_TEST_CASE(Points_basics)
{
    // From geometry
	PointGeometryPtr geometry = PointGeometry::create(Vector3::ZERO);
	MaterialPtr material = PointMaterial::create();
	PointsPtr object = Points::create(geometry, material);
	BOOST_TEST(object->geometry == geometry);
	BOOST_TEST(object->material == material);

    // From single point
    object = Points::create(Vector3::ZERO, material);
    BOOST_TEST(object != nullptr);

    // From vector
    std::vector<Vector3> points = {
        Vector3::XHAT,
        Vector3::YHAT
    };
    object = Points::create(points, material);
    BOOST_TEST(object != nullptr);

    // From initializer list
    object = Points::create({
        Vector3::XHAT,
        Vector3::YHAT
    }, material);
    BOOST_TEST(object != nullptr);
}

/**
 * Tests that points are applied to the state during scene traversal
 */
BOOST_AUTO_TEST_CASE(Points_traversal)
{
	PointGeometryPtr geometry = PointGeometry::create(Vector3::ZERO);
	MaterialPtr material = PointMaterial::create();
	PointsPtr points = Points::create(geometry, material);

	DrawState state;
	points->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(state.items.size() == 1);
	BOOST_TEST(state.items[0].variants.size() == 1);
	BOOST_TEST(state.items[0].variants[0].mode == RenderModes::MATERIAL);
}
