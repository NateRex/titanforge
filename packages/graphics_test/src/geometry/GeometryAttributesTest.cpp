#include <boost/test/unit_test.hpp>
#include <graphics/geometry/GeometryAttributes.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic constructors and accessors
 */
BOOST_AUTO_TEST_CASE(GeometryAttributes_basics)
{
	GeometryAttributes attrib;
	BOOST_TEST(!attrib.colors);
	BOOST_TEST(!attrib.uvs);

	attrib = { true, true };
	BOOST_TEST(attrib.colors);
	BOOST_TEST(attrib.uvs);
}

/**
 * Tests equality operators
 */
BOOST_AUTO_TEST_CASE(GeometryAttributes_equalsChecks)
{
	GeometryAttributes a1 = { false, false };
	BOOST_TEST(a1 == a1);

	GeometryAttributes a2 = { true, false };
	BOOST_TEST(a1 != a2);

	a2 = { false, true };
	BOOST_TEST(a1 != a2);
}

/**
 * Tests the ability to get the stride of vertices, given the attributes included with each
 * vertex
 */
BOOST_AUTO_TEST_CASE(GeometryAttributes_stride)
{
	GeometryAttributes a = { false, false };
	BOOST_TEST(a.getStride() == 3);

	a = { true, false };
	BOOST_TEST(a.getStride() == 7);

	a = { false, true };
	BOOST_TEST(a.getStride() == 5);

	a = { true, true };
	BOOST_TEST(a.getStride() == 9);
}