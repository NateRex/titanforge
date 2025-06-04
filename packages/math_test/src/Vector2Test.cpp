#include <boost/test/unit_test.hpp>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic constructors and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Vector2_basics)
{
    Vector2 vec;
    BOOST_TEST(equals(vec.x, 0, 1.0e-6));
    BOOST_TEST(equals(vec.y, 0, 1.0e-6));

    vec = Vector2(1.1, -2.2);
    BOOST_TEST(equals(vec.x, 1.1, 1.0e-6));
    BOOST_TEST(equals(vec.y, -2.2, 1.0e-6));

    Vector2 vec2(vec);
    BOOST_TEST(vec == vec2);

    Vector2 vec3(Vector3(1.1, -2.2, 3.3));
    BOOST_TEST(vec == vec3);
}

/**
 * Tests the isZero() method
 */
BOOST_AUTO_TEST_CASE(Vector2_isZero)
{
    Vector2 vec(0, 0);
    BOOST_TEST(vec.isZero());

    vec = Vector2(0.1, 0);
    BOOST_TEST(!vec.isZero());
    BOOST_TEST(vec.isZero(0.1));

    vec = Vector2(0, 0.1);
    BOOST_TEST(!vec.isZero());
    BOOST_TEST(vec.isZero(0.1));
}

/**
 * Tests the ability to compare vectors for equality
 */
BOOST_AUTO_TEST_CASE(Vector2_equalTo)
{
    Vector2 v1(1, 2);
    BOOST_TEST(v1.equalTo(v1));
    BOOST_TEST(v1 == v1);

    Vector2 v2(1.1, 2);
    BOOST_TEST(!v1.equalTo(v2));
    BOOST_TEST(v1 != v2);

    v2 = Vector2(1, 2.1);
    BOOST_TEST(!v1.equalTo(v2));
    BOOST_TEST(v1 != v2);
}

/**
 * Tests the getMagnitude() method
 */
BOOST_AUTO_TEST_CASE(Vector2_magnitude)
{
    Vector2 vec(3, -4);
    BOOST_TEST(vec.getMagnitude() == 5);
}

/**
 * Tests the normalize() method
 */
BOOST_AUTO_TEST_CASE(Vector2_normalize)
{
    Vector2 vec;
    Vector2 normalized = vec.normalize();
    BOOST_TEST(normalized.isZero());

    vec = Vector2(100, -42);
    normalized = vec.normalize();
    BOOST_TEST(equals(normalized.getMagnitude(), 1, 1.0e-12));

    Vector2 result;
    vec.normalize(&result);
    BOOST_TEST(result == normalized);
}

/**
 * Tests the ability to obtain the dot product of two vectors
 */
BOOST_AUTO_TEST_CASE(Vector2_dotProduct)
{
    Vector2 v1(1, -2);
    Vector2 v2(-4, 5);
    float dotProduct = v1.dot(v2);
    BOOST_TEST(dotProduct == v2.dot(v1));
    BOOST_TEST(dotProduct == -14);
}

/**
 * Tests the ability to obtain the cross product of two vectors
 */
BOOST_AUTO_TEST_CASE(Vector2_crossProduct)
{
    Vector2 v1(1, -2);
    Vector2 v2(-4, 5);
    float v1CrossV2 = v1.cross(v2);
    BOOST_TEST(equals(v1CrossV2, -3, 1.0e-12));

    float v2CrossV1 = v2.cross(v1);
    BOOST_TEST(equals(v2CrossV1, 3, 1.0e-12));   
}

/**
 * Tests uniform scaling of a vector
 */
BOOST_AUTO_TEST_CASE(Vector2_uniformScaling)
{
    Vector2 v1(-3, 8);
    Vector2 v2 = v1.scale(-2);
    BOOST_TEST(equals(v2.x, 6, 1.0e-12));
    BOOST_TEST(equals(v2.y, -16, 1.0e-12));

    Vector2 result;
    v1.scale(-2, &result);
    BOOST_TEST(v2 == result);
}

/**
 * Tests non-uniform scaling of a vector
 */
BOOST_AUTO_TEST_CASE(Vector2_nonUniformScaling)
{
    Vector2 v1(1, 2);
    Vector2 v2 = v1.scale(2, -3);
    BOOST_TEST(equals(v2.x, 2, 1.0e-12));
    BOOST_TEST(equals(v2.y, -6, 1.0e-12));

    Vector2 result;
    v1.scale(2, -3, &result);
    BOOST_TEST(v2 == result);
}

/**
 * Tests the operator for subtracting a vector from another vector
 */
BOOST_AUTO_TEST_CASE(Vector2_subtraction)
{
    Vector2 v1(4, 1002);
    BOOST_TEST(v1.minus(v1).isZero());

    Vector2 v2 = Vector2(2.5, 1001.9);
    Vector2 subtraction = v1.minus(v2);
    BOOST_TEST(equals(subtraction.x, 1.5, 1.0e-2));
    BOOST_TEST(equals(subtraction.y, 0.1, 1.0e-2));

    Vector2 result;
    v1.minus(v2, &result);
    BOOST_TEST(result == subtraction);
}

