#include <common/Utils.h>
#include <common/Constants.h>
#include <catch.hpp>

/**
 * Tests equals
 */
TEST_CASE("equals", "[Utils]")
{
    REQUIRE(equals(1.101, 1.101, 0));
    REQUIRE(equals(1.101, 0, 1.101));
    REQUIRE_FALSE(equals(1.101, 0, 1.100));
}

/**
 * Tests safeDelete
 */
TEST_CASE("safeDelete", "[Utils]")
{
    REQUIRE_NOTHROW(safeDelete(nullptr));

    int* ptr = new int;
    REQUIRE_NOTHROW(safeDelete(ptr));
}

/**
 * Tests rad2Deg
 */
TEST_CASE("rad2Deg", "[Utils]")
{
    REQUIRE(rad2Deg(0) == 0);
    REQUIRE(rad2Deg(-PI / 2) == -90);
    REQUIRE(rad2Deg(PI) == 180);
    REQUIRE(rad2Deg(-2 * PI) == -360);
}

/**
 * Tests deg2Rad
 */
TEST_CASE("deg2Rad", "[Utils]")
{
    REQUIRE(deg2Rad(0) == 0);
    REQUIRE(deg2Rad(-90) == -PI / 2);
    REQUIRE(deg2Rad(180) == PI);
    REQUIRE(deg2Rad(-360) == -2 * PI);
}

/**
 * Tests getOrDefault
 */
TEST_CASE("getOrDefault", "[Utils]")
{
    double one = 1.;
    double* ptr = nullptr;
    REQUIRE(&getOrDefault(ptr, one) == &one);

    double two = 2.;
    ptr = &two;
    REQUIRE(&getOrDefault(ptr, one) == &two);
}