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
 * Tests resolvePath
 */
BOOST_AUTO_TEST_CASE(Utils_resolvePath)
{
    std::string path = resolvePath("one/2/myFile.txt");
    BOOST_TEST(path.find("bin") != std::string::npos);
    BOOST_TEST(path.find("one") != std::string::npos);
    BOOST_TEST(path.find("2") != std::string::npos);
    BOOST_TEST(path.find("myFile.txt") != std::string::npos);
}

/**
 * Tests checkSuffix
 */
BOOST_AUTO_TEST_CASE(Utils_checkSuffix)
{
    BOOST_TEST(!checkSuffix("abc", "abcd"));
    BOOST_TEST(!checkSuffix("abc", "ab"));
    BOOST_TEST(checkSuffix("abc", "abc"));
    BOOST_TEST(checkSuffix("abcdefg", "fg"));
}

/**
 * Tests getOrDefault
 */
BOOST_AUTO_TEST_CASE(Utils_getOrDefault)
{
    float one = 1.;
    float* ptr = nullptr;
    BOOST_TEST((getOrDefault(ptr, one) == one));
    BOOST_TEST((getOrDefault(ptr, 1.f) == 1.));

    float two = 2.;
    ptr = &two;
    BOOST_TEST((getOrDefault(ptr, one) == two));
    BOOST_TEST((getOrDefault(ptr, 1.f) == two));
}
