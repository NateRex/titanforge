#include <boost/test/unit_test.hpp>
#include <common/exceptions/InstantiationException.h>
#include <cstring>

/**
 * Tests the ability to create an instance of the exception
 */
BOOST_AUTO_TEST_CASE(InstantiationException_basics)
{
    InstantiationException ex("Test exception");
    BOOST_REQUIRE_THROW(throw ex, InstantiationException);

    const char* str = ex.what();
    BOOST_TEST(strcmp("Test exception", str) == 0);
}