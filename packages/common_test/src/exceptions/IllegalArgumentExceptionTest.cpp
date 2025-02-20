#include <boost/test/unit_test.hpp>
#include <common/exceptions/IllegalArgumentException.h>
#include <cstring>

/**
 * Tests the ability to create an instance of the exception
 */
BOOST_AUTO_TEST_CASE(IllegalArgumentException_basics)
{
    IllegalArgumentException ex("Test exception");
    BOOST_REQUIRE_THROW(throw ex, IllegalArgumentException);

    const char* str = ex.what();
    BOOST_TEST(strcmp("Test exception", str) == 0);
}