#include <boost/test/unit_test.hpp>
#include <graphics/primitives/PrimitiveAttributes.h>

/**
 * Tests equality operators
 */
BOOST_AUTO_TEST_CASE(PrimitiveAttributes_equalsChecks)
{
	PrimitiveAttributes a1(false);
	BOOST_TEST(!a1.hasColor);
	BOOST_TEST((a1 == PrimitiveAttributes(false)));

	PrimitiveAttributes a2(true);
	BOOST_TEST(a2.hasColor);
	BOOST_TEST((a1 != a2));
}