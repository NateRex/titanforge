#include <geometry/Vector3.h>
#include <geometry/Vector2.h>
#include <common/Utils.h>
#include <catch.hpp>

/**
 * Tests the basic constructors and accessors of the class
 */
TEST_CASE("Basics", "[Vector3]")
{
    Vector3 vec;
    REQUIRE(vec.x == 0.);
    REQUIRE(vec.y == 0.);
    REQUIRE(vec.z == 0.);

    vec = Vector3(1.1, -2.2, 3.3);
    REQUIRE(vec.x == 1.1);
    REQUIRE(vec.y == -2.2);
    REQUIRE(vec.z == 3.3);

    Vector3 vec2(vec);
    REQUIRE(vec == vec2);

    Vector3 vec3(Vector2(1.1, -2.2));
    REQUIRE(vec.x == vec3.x);
    REQUIRE(vec.y == vec3.y);
    REQUIRE(vec3.z == 0.);
}

/**
 * Tests the isZero() method
 */
TEST_CASE("IsZero", "[Vector3]")
{
    Vector3 vec(0, 0, 0);
    REQUIRE(vec.isZero());

    vec = Vector3(0.1, 0, 0);
    REQUIRE_FALSE(vec.isZero());
    REQUIRE(vec.isZero(0.1));

    vec = Vector3(0, 0.1, 0);
    REQUIRE_FALSE(vec.isZero());
    REQUIRE(vec.isZero(0.1));

    vec = Vector3(0, 0, 0.1);
    REQUIRE_FALSE(vec.isZero());
    REQUIRE(vec.isZero(0.1));
}

/**
 * Tests the ability to compare vectors for equality
 */
TEST_CASE("EqualTo", "[Vector3]")
{
    Vector3 v1(1., 2., 3.);
    REQUIRE(v1.equalTo(v1));
    REQUIRE(v1 == v1);

    Vector3 v2(1.1, 2., 3.);
    REQUIRE_FALSE(v1.equalTo(v2));
    REQUIRE_FALSE(v1 == v2);

    v2 = Vector3(1., 2.1, 3.);
    REQUIRE_FALSE(v1.equalTo(v2));
    REQUIRE_FALSE(v1 == v2);

    v2 = Vector3(1., 2., 3.1);
    REQUIRE_FALSE(v1.equalTo(v2));
    REQUIRE_FALSE(v1 == v2);
}

/**
 * Tests the getMagnitude() method
 */
TEST_CASE("Magnitude", "[Vector3]")
{
    Vector3 vec(1, 2, 3);
    REQUIRE(equals(vec.getMagnitude(), 3.741657386, 1.0e-9));
}

/**
 * Tests the normalize() method
 */
TEST_CASE("Normalize", "[Vector3]")
{
    Vector3 vec;
    Vector3 normalized = vec.normalize();
    REQUIRE(normalized.isZero());

    vec = Vector3(1, 2, 3);
    normalized = vec.normalize();
    REQUIRE(normalized.equalTo(Vector3(0.267261241, 0.534522483, 0.801783725), 1.0e-9));

    Vector3 result;
    vec.normalize(&result);
    REQUIRE(result == normalized);
}

/**
 * Tests the ability to obtain the dot product of two vectors
 */
TEST_CASE("DotProduct", "[Vector3]")
{
    Vector3 v1(1, -2, 3);
    Vector3 v2(-4, 5, -6);
    double dotProduct = v1.dot(v2);
    REQUIRE(dotProduct == v2.dot(v1));
    REQUIRE(dotProduct == -32.);
}

/**
 * Tests the ability to obtain the cross product of two vectors
 */
TEST_CASE("CrossProduct", "[Vector3]")
{
    Vector3 v1(1, -2, 3);
    Vector3 v2(-4, 5, -6);
    Vector3 v1CrossV2 = v1.cross(v2);
    REQUIRE(v1CrossV2.equalTo(Vector3(-3, -6, -3), 1.0e-12));

    Vector3 v2CrossV1 = v2.cross(v1);
    REQUIRE(v2CrossV1.equalTo(Vector3(3, 6, 3), 1.0e-12));

    Vector3 result;
    v2.cross(v1, &result);
    REQUIRE(result == v2CrossV1);
}

/**
 * Tests the operator for scaling a vector
 */
TEST_CASE("Scaling", "[Vector3]")
{
    Vector3 v1(-3, 8, 1);
    Vector3 v2 = v1.scale(-2);
    REQUIRE(equals(v2.x, 6, 1.0e-12));
    REQUIRE(equals(v2.y, -16, 1.0e-12));
    REQUIRE(equals(v2.z, -2, 1.0e-12));

    Vector3 result;
    v1.scale(-2, &result);
    REQUIRE(result == v2);
}

/**
 * Tests the operator for subtracting a vector from another vector
 */
TEST_CASE("Subtraction", "[Vector3]")
{
    Vector3 v1(4, 1002, -101.5);
    REQUIRE(v1.minus(v1).isZero());

    Vector3 v2(2.5, 1001.9, -5);
    Vector3 subtraction = v1.minus(v2);
    REQUIRE(equals(subtraction.x, 1.5, 1.0e-12));
    REQUIRE(equals(subtraction.y, 0.1, 1.0e-12));
    REQUIRE(equals(subtraction.z, -96.5, 1.0e-12));

    Vector3 result;
    v1.minus(v2, &result);
    REQUIRE(result == subtraction);
}