#include <boost/test/unit_test.hpp>
#include <math/Vector3.h>
#include <math/Vector2.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic constructors and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Vector3_basics)
{
    Vector3 vec;
    BOOST_TEST(equals(vec.x, 0., 1.0e-6));
    BOOST_TEST(equals(vec.y, 0., 1.0e-6));
    BOOST_TEST(equals(vec.z, 0., 1.0e-6));

    vec = Vector3(1.1, -2.2, 3.3);
    BOOST_TEST(equals(vec.x, 1.1, 1.0e-6));
    BOOST_TEST(equals(vec.y, -2.2, 1.0e-6));
    BOOST_TEST(equals(vec.z, 3.3, 1.0e-6));

    Vector3 vec2(vec);
    BOOST_TEST(vec == vec2);

    Vector3 vec3(Vector2(1.1, -2.2));
    BOOST_TEST(vec.x == vec3.x);
    BOOST_TEST(vec.y == vec3.y);
    BOOST_TEST(vec3.z == 0.);
}

/**
 * Tests the isZero() method
 */
BOOST_AUTO_TEST_CASE(Vector3_isZero)
{
    Vector3 vec(0, 0, 0);
    BOOST_TEST(vec.isZero());

    vec = Vector3(0.1, 0, 0);
    BOOST_TEST(!vec.isZero());
    BOOST_TEST(vec.isZero(0.1));

    vec = Vector3(0, 0.1, 0);
    BOOST_TEST(!vec.isZero());
    BOOST_TEST(vec.isZero(0.1));

    vec = Vector3(0, 0, 0.1);
    BOOST_TEST(!vec.isZero());
    BOOST_TEST(vec.isZero(0.1));
}

/**
 * Tests the ability to compare vectors for equality
 */
BOOST_AUTO_TEST_CASE(Vector3_equalTo)
{
    Vector3 v1(1., 2., 3.);
    BOOST_TEST(v1.equalTo(v1));
    BOOST_TEST(v1 == v1);

    Vector3 v2(1.1, 2., 3.);
    BOOST_TEST(!v1.equalTo(v2));
    BOOST_TEST(v1 != v2);

    v2 = Vector3(1., 2.1, 3.);
    BOOST_TEST(!v1.equalTo(v2));
    BOOST_TEST(v1 != v2);

    v2 = Vector3(1., 2., 3.1);
    BOOST_TEST(!v1.equalTo(v2));
    BOOST_TEST(v1 != v2);
}

/**
 * Tests the getMagnitude() method
 */
BOOST_AUTO_TEST_CASE(Vector3_magnitude)
{
    Vector3 vec(1, 2, 3);
    BOOST_TEST(equals(vec.getMagnitude(), 3.741657386, 1.0e-9));
}

/**
 * Tests the normalize() method
 */
BOOST_AUTO_TEST_CASE(Vector3_normalize)
{
    Vector3 vec;
    Vector3 normalized = vec.normalize();
    BOOST_TEST(normalized.isZero());

    vec = Vector3(1, 2, 3);
    normalized = vec.normalize();
    BOOST_TEST(normalized.equalTo(Vector3(0.2672, 0.5345, 0.8017), 1.0e-4));

    Vector3 result;
    vec.normalize(&result);
    BOOST_TEST(result == normalized);
}

/**
 * Tests the ability to obtain the dot product of two vectors
 */
BOOST_AUTO_TEST_CASE(Vector3_dotProduct)
{
    Vector3 v1(1, -2, 3);
    Vector3 v2(-4, 5, -6);
    float dotProduct = v1.dot(v2);
    BOOST_TEST(dotProduct == v2.dot(v1));
    BOOST_TEST(dotProduct == -32.);
}

/**
 * Tests the ability to obtain the cross product of two vectors
 */
BOOST_AUTO_TEST_CASE(Vector3_crossProduct)
{
    Vector3 v1(1, -2, 3);
    Vector3 v2(-4, 5, -6);
    Vector3 v1CrossV2 = v1.cross(v2);
    BOOST_TEST(v1CrossV2.equalTo(Vector3(-3, -6, -3), 1.0e-12));

    Vector3 v2CrossV1 = v2.cross(v1);
    BOOST_TEST(v2CrossV1.equalTo(Vector3(3, 6, 3), 1.0e-12));

    Vector3 result;
    v2.cross(v1, &result);
    BOOST_TEST(result == v2CrossV1);
}

/**
 * Tests the ability to obtain the angle between two vectors
 */
BOOST_AUTO_TEST_CASE(Vector3_angleBetween)
{
    Vector3 v1(1, 0, 0);

    Vector3 v2(0, 1, 0);
    float angle = v1.angleBetween(v2);
    BOOST_TEST(equals(angle, deg2Rad(90.f), 1.0e-12));

    v2 = Vector3(0, 0, 1);
    angle = v1.angleBetween(v2);
    BOOST_TEST(equals(angle, deg2Rad(90.f), 1.0e-12));

    v2 = Vector3(-1, 0, 0);
    angle = v1.angleBetween(v2);
    BOOST_TEST(equals(angle, deg2Rad(180.f), 1.0e-12));

    v2 = Vector3(1, 1, 0);
    angle = v1.angleBetween(v2);
    BOOST_TEST(equals(angle, deg2Rad(45.f), 1.0e-12));

    v2 = Vector3(1, 0, -1);
    angle = v1.angleBetween(v2);
    BOOST_TEST(equals(angle, deg2Rad(45.f), 1.0e-12));
}

/**
 * Tests uniform scaling of a vector
 */
BOOST_AUTO_TEST_CASE(Vector3_uniformScaling)
{
    Vector3 v1(-3, 8, 1);
    Vector3 v2 = v1.scale(-2);
    BOOST_TEST(equals(v2.x, 6, 1.0e-12));
    BOOST_TEST(equals(v2.y, -16, 1.0e-12));
    BOOST_TEST(equals(v2.z, -2, 1.0e-12));

    Vector3 result;
    v1.scale(-2, &result);
    BOOST_TEST(result == v2);
}

/**
 * Tests non-uniform scaling of a vector
 */
BOOST_AUTO_TEST_CASE(Vector3_nonUniformScaling)
{
    Vector3 v1(1, 2, 3);
    Vector3 v2 = v1.scale(-2, 3, -4);
    BOOST_TEST(equals(v2.x, -2, 1.0e-12));
    BOOST_TEST(equals(v2.y, 6, 1.0e-12));
    BOOST_TEST(equals(v2.z, -12, 1.0e-12));

    Vector3 result;
    v1.scale(-2, 3, -4, &result);
    BOOST_TEST(result == v2);
}

/**
 * Tests the operator for subtracting a vector from another vector
 */
BOOST_AUTO_TEST_CASE(Vector3_subtraction)
{
    Vector3 v1(4, 1002, -101.5);
    BOOST_TEST(v1.minus(v1).isZero());

    Vector3 v2(2.5, 1001.9, -5);
    Vector3 subtraction = v1.minus(v2);
    BOOST_TEST(equals(subtraction.x, 1.5, 1.0e-2));
    BOOST_TEST(equals(subtraction.y, 0.1, 1.0e-2));
    BOOST_TEST(equals(subtraction.z, -96.5, 1.0e-2));

    Vector3 result;
    v1.minus(v2, &result);
    BOOST_TEST(result == subtraction);
}