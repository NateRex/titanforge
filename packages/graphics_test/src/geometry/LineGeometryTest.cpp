#include <boost/test/unit_test.hpp>
#include <graphics/geometry/LineGeometry.h>
#include <math/Vector3.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation of a line geometry consisting of disjoint segments
 */
BOOST_AUTO_TEST_CASE(LineGeometry_segments)
{
    std::vector<Vector3> points = {
        Vector3(0.f, 0.f, 0.f),
        Vector3(1.f, 0.f, 0.f),
        Vector3(1.f, 1.f, 0.f),
        Vector3(0.f, 1.f, 0.f)
    };
    LineGeometryPtr geom = LineGeometry::createSegments(points.data(), points.size());
    BOOST_TEST(geom->type == PrimitiveType::LINE_SEGMENTS);
    BOOST_TEST(geom->size() == 4);

	GeometryAttributes attributes = geom->getAttributes();
    BOOST_TEST(attributes.primitiveType == PrimitiveType::LINE_SEGMENTS);
    BOOST_TEST(!attributes.indices);
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.uvs);
	BOOST_TEST(!attributes.colors);
}

/**
 * Tests creation of a geometry representing a connected polyline
 */
BOOST_AUTO_TEST_CASE(LineGeometry_polyline)
{
    std::vector<Vector3> points = {
        Vector3(0.f, 0.f, 0.f),
        Vector3(1.f, 0.f, 0.f),
        Vector3(1.f, 1.f, 0.f),
        Vector3(0.f, 1.f, 0.f)
    };

    // Open polyline
    LineGeometryPtr geom = LineGeometry::createPolyline(points.data(), points.size());
    BOOST_TEST(geom->type == PrimitiveType::POLYLINE);
    BOOST_TEST(geom->size() == 4);

    // Closed polyline
    geom = LineGeometry::createPolyline(points.data(), points.size(), true);
    BOOST_TEST(geom->type == PrimitiveType::CLOSED_POLYLINE);
    BOOST_TEST(geom->size() == 4);

	GeometryAttributes attributes = geom->getAttributes();
    BOOST_TEST(!attributes.indices);
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.uvs);
	BOOST_TEST(!attributes.colors);
}