#include <common/exceptions/IllegalArgumentException.h>
#include <cstring>
#include <catch.hpp>

/**
 * Tests the ability to create an instance of the exception
 */
TEST_CASE("basics", "[IllegalArgumentException]")
{
    IllegalArgumentException ex("Test exception");
    REQUIRE_THROWS_AS(throw ex, IllegalArgumentException);

    const char* str = ex.what();
    REQUIRE(strcmp("Test exception", str) == 0);
}
