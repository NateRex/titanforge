#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <catch.hpp>

/**
 * Tests the basic constructors and accessors of the class
 */
TEST_CASE("basics", "[Matrix3]")
{
	Matrix3 m;
	REQUIRE(m.isIdentity());

	m = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
	for (int i = 0; i < 9; i++)
	{
		REQUIRE(m[i] == i + 1);
	}

	Matrix3 m2 = Matrix3(m);
	REQUIRE(m2 == m);
}

/**
 * Tests construction of a matrix using row and column vectors
 */
TEST_CASE("fromRowsAndColumns", "[Matrix3]")
{
	Matrix3 expected = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);

	Matrix3 m;
	Matrix3::fromRows(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(7, 8, 9), &m);
	REQUIRE(m == expected);

	Matrix3::fromColumns(Vector3(1, 4, 7), Vector3(2, 5, 8), Vector3(3, 6, 9), &m);
	REQUIRE(m == expected);
}

/**
 * Tests construction of a matrix using rotation angles
 */
TEST_CASE("fromRotations", "[Matrix3]")
{
	Matrix3 rot;
	Vector3 v;

	Matrix3::fromXRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(0, 0, 1), &v);
	REQUIRE(v.equalTo(Vector3(0, -1, 0), 1.0e-12));

	Matrix3::fromYRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(1, 0, 0), &v);
	REQUIRE(v.equalTo(Vector3(0, 0, -1), 1.0e-12));

	Matrix3::fromZRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(0, 1, 0), &v);
	REQUIRE(v.equalTo(Vector3(-1, 0, 0), 1.0e-12));
}