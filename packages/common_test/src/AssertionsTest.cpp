#include <boost/test/unit_test.hpp>
#include <common/Assertions.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <unordered_map>
#include <map>

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
	BOOST_REQUIRE_THROW(assertNotNull(ptr, "Ptr cannot be null"), NullPointerException);
	BOOST_REQUIRE_THROW(assertNotNull(ptr, "Ptr cannot be null", increment), NullPointerException);
	BOOST_TEST(value == 2);

	ptr = &value;
	BOOST_REQUIRE_NO_THROW(assertNotNull(ptr, "Ptr cannot be null"));
}

/**
 * Tests assertTrue and assertFalse functions
 */
BOOST_AUTO_TEST_CASE(Assertions_trueVsFalse)
{
	BOOST_REQUIRE_NO_THROW(assertTrue(true));
	BOOST_REQUIRE_THROW(assertTrue(false), IllegalArgumentException);
	BOOST_REQUIRE_NO_THROW(assertFalse(false));
	BOOST_REQUIRE_THROW(assertFalse(true), IllegalArgumentException);
}

/**
 * Tests assertEquals
 */
BOOST_AUTO_TEST_CASE(Assertions_assertEquals)
{
	BOOST_REQUIRE_NO_THROW(assertEquals(1, 1));
	BOOST_REQUIRE_THROW(assertEquals(2, 1), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertEquals(1, 2), IllegalArgumentException);
}

/**
 * Tests assertGreaterThan
 */
BOOST_AUTO_TEST_CASE(Assertions_greaterThan)
{
	BOOST_REQUIRE_NO_THROW(assertGreaterThan(2, 1));
	BOOST_REQUIRE_THROW(assertGreaterThan(1, 1), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertGreaterThan(-1, 1), IllegalArgumentException);
}

/**
 * Tests assertGreaterThanOrEqual
 */
BOOST_AUTO_TEST_CASE(Assertions_greaterThanOrEqual)
{
	BOOST_REQUIRE_NO_THROW(assertGreaterThanOrEqual(2, 1));
	BOOST_REQUIRE_NO_THROW(assertGreaterThanOrEqual(1, 1));
	BOOST_REQUIRE_THROW(assertGreaterThanOrEqual(-1, 1), IllegalArgumentException);
}

/**
 * Tests assertLessThan
 */
BOOST_AUTO_TEST_CASE(Assertions_lessThan)
{
	BOOST_REQUIRE_NO_THROW(assertLessThan(0, 1));
	BOOST_REQUIRE_THROW(assertLessThan(1, 1), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertLessThan(2, 1), IllegalArgumentException);
}

/**
 * Tests assertLessThanOrEqual
 */
BOOST_AUTO_TEST_CASE(Assertions_lessThanOrEqual)
{
	BOOST_REQUIRE_NO_THROW(assertLessThanOrEqual(0, 1));
	BOOST_REQUIRE_NO_THROW(assertLessThanOrEqual(1, 1));
	BOOST_REQUIRE_THROW(assertLessThanOrEqual(21, 1), IllegalArgumentException);
}

/**
 * Tests assertInRange
 */
BOOST_AUTO_TEST_CASE(Assertions_inRange)
{
	// Inclusive bounds
	BOOST_REQUIRE_NO_THROW(assertInRange(1, 0, 2));
	BOOST_REQUIRE_NO_THROW(assertInRange(0, 0, 2));
	BOOST_REQUIRE_NO_THROW(assertInRange(2, 0, 2));
	BOOST_REQUIRE_THROW(assertInRange(-1, 0, 2), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertInRange(3, 0, 2), IllegalArgumentException);

	// Exclusive bounds
	BOOST_REQUIRE_THROW(assertInRange(0, 0, 2, false), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertInRange(2, 0, 2, false), IllegalArgumentException);
}

/**
 * Tests assertions of whether or not a key exists in a map
 */
BOOST_AUTO_TEST_CASE(Assertions_mapAssertions)
{
	// Test with both ordered and unordered maps
	std::unordered_map<int, std::string> map1 = { {1, "apple"}, {2, "banana"}, {3, "cherry"} };
	std::map<int, std::string> map2 = { {1, "apple"}, {2, "banana"}, {3, "cherry"} };

	BOOST_REQUIRE_NO_THROW(assertKeyInMap(map1, 1));
	BOOST_REQUIRE_NO_THROW(assertKeyInMap(map2, 2));
	BOOST_REQUIRE_THROW(assertKeyInMap(map1, 4), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertKeyInMap(map2, 5), IllegalArgumentException);

	BOOST_REQUIRE_NO_THROW(assertKeyNotInMap(map1, 5));
	BOOST_REQUIRE_NO_THROW(assertKeyNotInMap(map2, 4));
	BOOST_REQUIRE_THROW(assertKeyNotInMap(map1, 3), IllegalArgumentException);
	BOOST_REQUIRE_THROW(assertKeyNotInMap(map2, 1), IllegalArgumentException);
}