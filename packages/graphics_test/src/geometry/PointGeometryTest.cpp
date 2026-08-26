#include <boost/test/unit_test.hpp>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/geometry/GeometryAttributes.h>

/**
 * Tests creation of a point geometry from a single point
 */
BOOST_AUTO_TEST_CASE(PointGeometry_fromSinglePoint)
{
	PointGeometryPtr geom = PointGeometry::create(Vector3(0.f, 1.f, 2.f));
	BOOST_TEST(geom->size() == 1);

	GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.uvs);
	BOOST_TEST(!attributes.colors);
}

/**
 * Tests creation of a point geometry from a vector of points
 */
BOOST_AUTO_TEST_CASE(PointGeometry_fromVector)
{
    std::vector<Vector3> points = {
        Vector3(0.f, 1.f, 2.f),
        Vector3(3.f, 4.f, 5.f)
    };
    PointGeometryPtr geom = PointGeometry::create(points);
    BOOST_TEST(geom->size() == 2);

    GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.uvs);
	BOOST_TEST(!attributes.colors);
}

/**
 * Tests creation of a point geometry from an initializer list of points
 */
BOOST_AUTO_TEST_CASE(PointGeometry_fromInitializerList)
{
    PointGeometryPtr geom = PointGeometry::create({
        Vector3(0.f, 1.f, 2.f),
        Vector3(3.f, 4.f, 5.f)
    });
    BOOST_TEST(geom->size() == 2);

    GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.uvs);
	BOOST_TEST(!attributes.colors);
}