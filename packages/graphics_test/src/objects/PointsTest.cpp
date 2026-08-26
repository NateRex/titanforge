#include <boost/test/unit_test.hpp>
#include <graphics/objects/Points.h>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/materials/PointMaterial.h>

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