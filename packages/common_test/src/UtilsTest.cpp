#include <boost/test/unit_test.hpp>
#include <common/Utils.h>
#include <common/Constants.h>

/**
 * Tests equals
 */
BOOST_AUTO_TEST_CASE(Utils_equals)
{
    BOOST_TEST(equals(1.101, 1.101, 0));
    BOOST_TEST(equals(1.101, 0, 1.101));
    BOOST_TEST(!equals(1.101, 0, 1.100));
}

/**
 * Tests safeDelete
 */
BOOST_AUTO_TEST_CASE(Utils_safeDelete)
{
    int* ptr = new int;
    BOOST_REQUIRE_NO_THROW(safeDelete(ptr));

    ptr = nullptr;
    BOOST_REQUIRE_NO_THROW(safeDelete(ptr));
}

/**
 * Tests rad2Deg
 */
BOOST_AUTO_TEST_CASE(Utils_rad2Deg)
{
    BOOST_TEST(rad2Deg(0) == 0);
    BOOST_TEST(rad2Deg(-PI / 2) == -90);
    BOOST_TEST(rad2Deg(PI) == 180);
    BOOST_TEST(rad2Deg(-2 * PI) == -360);
}

/**
 * Tests deg2Rad
 */
BOOST_AUTO_TEST_CASE(Utils_deg2Rad)
{
    BOOST_TEST(deg2Rad(0) == 0);
    BOOST_TEST(deg2Rad(-90) == -PI / 2);
    BOOST_TEST(deg2Rad(180) == PI);
    BOOST_TEST(deg2Rad(-360) == -2 * PI);
}

/**
 * Tests getOrDefault
 */
BOOST_AUTO_TEST_CASE(Utils_getOrDefault)
{
    double one = 1.;
    double* ptr = nullptr;
    double value = getOrDefault(ptr, one);
    BOOST_TEST(value == one);

    double two = 2.;
    ptr = &two;
    value = getOrDefault(ptr, one);
    BOOST_TEST(value == two);
}
