#include <boost/test/unit_test.hpp>
#include <graphics/primitives/Primitive.h>
#include <common/Utils.h>

/**
 * Tests the basic construction of vertex attributes
 */
BOOST_AUTO_TEST_CASE(VertexAttributes_basics)
{
	VertexAttributes attrib;
	BOOST_TEST(!attrib.hasColor);
	BOOST_TEST(!attrib.hasTexture);

	attrib = { true, true };
	BOOST_TEST(attrib.hasColor);
	BOOST_TEST(attrib.hasTexture);
}

/**
 * Tests equality operators
 */
BOOST_AUTO_TEST_CASE(VertexAttributes_equalsChecks)
{
	VertexAttributes a1 = { false, false };
	BOOST_TEST(a1 == a1);

	VertexAttributes a2 = { true, false };
	BOOST_TEST(a1 != a2);

	a2 = { false, true };
	BOOST_TEST(a1 != a2);
}