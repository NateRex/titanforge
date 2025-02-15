#include <common/exceptions/OutOfBoundsException.h>
#include <cstring>
#include <catch.hpp>

/**
 * Tests the ability to create an instance of the exception
 */
TEST_CASE("basics", "[OutOfBoundsException]")
{
    OutOfBoundsException ex("Test exception");
    REQUIRE_THROWS_AS(throw ex, OutOfBoundsException);

    const char* str = ex.what();
    REQUIRE(strcmp("Test exception", str) == 0);
}
