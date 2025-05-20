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

    float two = 2.;
    ptr = &two;
    BOOST_TEST((getOrDefault(ptr, one) == two));
}

/**
 * Tests casting of pointers
 */
BOOST_AUTO_TEST_CASE(Utils_cast)
{
    // Runtime error can be casted to parent exception class
    std::shared_ptr<std::exception> ptr1 = std::make_shared<std::runtime_error>("");
    BOOST_REQUIRE_NO_THROW(cast<std::runtime_error>(ptr1));

    // bad_alloc and allocator have no polymorphic relationship
    std::shared_ptr<std::bad_alloc> ptr2 = std::make_shared<std::bad_alloc>();
    BOOST_REQUIRE_THROW(cast<std::allocator<char>>(ptr2), IllegalStateException);
}