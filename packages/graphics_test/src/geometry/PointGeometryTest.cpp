#include <boost/test/unit_test.hpp>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <math/Vector3.h>

/**
 * Tests creation of a point geometry
 */
BOOST_AUTO_TEST_CASE(PointGeometry_basics)
{
    // From single point
	PointGeometryPtr geom = PointGeometry::create(Vector3(0.f, 1.f, 2.f));
	BOOST_TEST(geom->size() == 1);

    // From list
    std::vector<Vector3> points = {
        Vector3(0.f, 1.f, 2.f),
        Vector3(3.f, 4.f, 5.f)
    };
    geom = PointGeometry::create(points.data(), points.size());
    BOOST_TEST(geom->size() == 2);

	GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.uvs);
	BOOST_TEST(!attributes.colors);
}