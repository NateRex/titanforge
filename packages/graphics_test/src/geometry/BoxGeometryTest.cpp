#include <boost/test/unit_test.hpp>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/geometry/GeometryAttributes.h>

/**
 * Tests the basic constructors, mutators, and accessors of the class
 */
BOOST_AUTO_TEST_CASE(BoxGeometry_basics)
{
	BoxGeometryPtr geom = BoxGeometry::create(2.f, 4.f, 8.f);
	BOOST_TEST(geom->size() == 36);

	GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(attributes.uvs);
	BOOST_TEST(!attributes.colors);
}