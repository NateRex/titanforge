#include <boost/test/unit_test.hpp>
#include <common/Assertions.h>

/**
 * Tests assertNotNull
 */
BOOST_AUTO_TEST_CASE(Assertions_notNull)
{
	int value = 1;
	std::function<void()> increment = [&value]() {
		value++;
	};

	int* ptr = nullptr;
	BOOST_REQUIRE_THROW(assertNotNull(ptr), NullPointerException);
	BOOST_REQUIRE_THROW(assertNotNull(ptr, increment), NullPointerException);
	BOOST_TEST(value == 2);

	ptr = &value;
	BOOST_REQUIRE_NO_THROW(assertNotNull(ptr));
}