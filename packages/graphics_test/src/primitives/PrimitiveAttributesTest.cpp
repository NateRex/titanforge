#include <boost/test/unit_test.hpp>
#include <graphics/primitives/PrimitiveAttributes.h>
#include <common/Utils.h>

/**
 * Tests the basic construction of primitive attributes
 */
BOOST_AUTO_TEST_CASE(PrimitiveAttributes_basics)
{
	PrimitiveAttributes attrib;
	BOOST_TEST(!attrib.hasColor);
	BOOST_TEST(!attrib.hasTextureCoords);

	attrib = PrimitiveAttributes(true, true);
	BOOST_TEST(attrib.hasColor);
	BOOST_TEST(attrib.hasTextureCoords);
}

/**
 * Tests equality operators
 */
BOOST_AUTO_TEST_CASE(PrimitiveAttributes_equalsChecks)
{
	PrimitiveAttributes attrib(false, false);
	BOOST_TEST(attrib == PrimitiveAttributes(false, false));
	BOOST_TEST(attrib != PrimitiveAttributes(true, false));
	BOOST_TEST(attrib != PrimitiveAttributes(false, true));
}