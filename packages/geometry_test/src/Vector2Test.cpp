#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <catch.hpp>

/**
 * Tests the basic constructors and accessors of the class
 */
TEST_CASE("Basics", "[Vector2]")
{
    Vector2 vec;
    REQUIRE(vec.x == 0);
    REQUIRE(vec.y == 0);

    vec = Vector2(1.1, -2.2);
    REQUIRE(vec.x == 1.1);
    REQUIRE(vec.y == -2.2);

    Vector2 vec2(vec);
    REQUIRE(vec == vec2);

    Vector2 vec3(Vector3(1.1, -2.2, 3.3));
    REQUIRE(vec == vec3);
}

/**
 * Tests the isZero() method
 */
TEST_CASE("IsZero", "[Vector2]")
{
    Vector2 vec(0, 0);
    REQUIRE(vec.isZero());

    vec = Vector2(0.1, 0);
    REQUIRE_FALSE(vec.isZero());
    REQUIRE(vec.isZero(0.1));

    vec = Vector2(0, 0.1);
    REQUIRE_FALSE(vec.isZero());
    REQUIRE(vec.isZero(0.1));
}

/**
 * Tests the ability to compare vectors for equality
 */
TEST_CASE("EqualTo", "[Vector2]")
{
    Vector2 v1(1, 2);
    REQUIRE(v1.equalTo(v1));
    REQUIRE(v1 == v1);

    Vector2 v2(1.1, 2);
    REQUIRE_FALSE(v1.equalTo(v2));
    REQUIRE_FALSE(v1 == v2);

    v2 = Vector2(1, 2.1);
    REQUIRE_FALSE(v1.equalTo(v2));
    REQUIRE_FALSE(v1 == v2);
}

/**
 * Tests the getMagnitude() method
 */
TEST_CASE("Magnitude", "[Vector2]")
{
    Vector2 vec(3, -4);
    REQUIRE(vec.getMagnitude() == 5);
}

/**
 * Tests the normalize() method
 */
TEST_CASE("Normalize", "[Vector2]")
{
    Vector2 vec;
    Vector2 normalized = vec.normalize();
    REQUIRE(normalized.isZero());

    vec = Vector2(100, -42);
    normalized = vec.normalize();
    REQUIRE(equals(normalized.getMagnitude(), 1, 1.0e-12));

    Vector2 result;
    vec.normalize(&result);
    REQUIRE(result == normalized);
}

/**
 * Tests the ability to obtain the dot product of two vectors
 */
TEST_CASE("DotProduct", "[Vector2]")
{
    Vector2 v1(1, -2);
    Vector2 v2(-4, 5);
    double dotProduct = v1.dot(v2);
    REQUIRE(dotProduct == v2.dot(v1));
    REQUIRE(dotProduct == -14);
}

/**
 * Tests the ability to obtain the cross product of two vectors
 */
TEST_CASE("CrossProduct", "[Vector2]")
{
    Vector2 v1(1, -2);
    Vector2 v2(-4, 5);
    double v1CrossV2 = v1.cross(v2);
    REQUIRE(equals(v1CrossV2, -3, 1.0e-12));

    double v2CrossV1 = v2.cross(v1);
    REQUIRE(equals(v2CrossV1, 3, 1.0e-12));   
}

/**
 * Tests the operator for scaling a vector
 */
TEST_CASE("Scaling", "[Vector2]")
{
    Vector2 v1(-3, 8);
    Vector2 v2 = v1.scale(-2);
    REQUIRE(equals(v2.x, 6, 1.0e-12));
    REQUIRE(equals(v2.y, -16, 1.0e-12));

    Vector2 result;
    v1.scale(-2, &result);
    REQUIRE(v2 == result);
}

/**
 * Tests the operator for subtracting a vector from another vector
 */
TEST_CASE("Subtraction", "[Vector2]")
{
    Vector2 v1(4, 1002);
    REQUIRE(v1.minus(v1).isZero());

    Vector2 v2 = Vector2(2.5, 1001.9);
    Vector2 subtraction = v1.minus(v2);
    REQUIRE(equals(subtraction.x, 1.5, 1.0e-12));
    REQUIRE(equals(subtraction.y, 0.1, 1.0e-12));

    Vector2 result;
    v1.minus(v2, &result);
    REQUIRE(result == subtraction);
}

