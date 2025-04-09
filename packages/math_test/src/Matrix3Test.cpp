#include <boost/test/unit_test.hpp>
#include <math/Matrix3.h>
#include <math/Vector3.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>
#include <common/Constants.h>

/**
 * Tests the basic constructors and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Matrix3_basics)
{
	Matrix3 m;
	BOOST_TEST(m.isIdentity());

	m = Matrix3(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f);
	for (int i = 0; i < 9; i++)
	{
		BOOST_TEST(m[i] == i + 1);
		BOOST_TEST(m.getValues()[i] == i + 1);
	}

	Matrix3 m2 = Matrix3(m);
	BOOST_TEST(m2 == m);
}

/**
 * Tests construction of a matrix using row and column vectors
 */
BOOST_AUTO_TEST_CASE(Matrix3_fromRowsAndColumns)
{
	Matrix3 expected = Matrix3(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f);

	Matrix3 m;
	Matrix3::fromRows(Vector3(1.f, 2.f, 3.f), Vector3(4.f, 5.f, 6.f), Vector3(7.f, 8.f, 9.f), &m);
	BOOST_TEST(m == expected);

	Matrix3::fromColumns(Vector3(1.f, 4.f, 7.f), Vector3(2.f, 5.f, 8.f), Vector3(3.f, 6.f, 9.f), &m);
	BOOST_TEST(m == expected);
}

/**
 * Tests construction of a matrix using rotation angles around an arbitrary axis
 */
BOOST_AUTO_TEST_CASE(Matrix3_fromArbitraryAxisRotation)
{
	BOOST_TEST(Matrix3::fromRotation(Vector3(1.f, 0.f, 0.f), PI) == Matrix3::fromXRotation(PI));
	BOOST_TEST(Matrix3::fromRotation(Vector3(0.f, 1.f, 0.f), PI) == Matrix3::fromYRotation(PI));
	BOOST_TEST(Matrix3::fromRotation(Vector3(0.f, 0.f, 1.f), PI) == Matrix3::fromZRotation(PI));

	Matrix3 rot = Matrix3::fromRotation(Vector3(0.662f, 0.2f, 0.722f), deg2Rad(45.f));
	Vector3 v(-1.f, 2.f, 4.f);

	rot.multiply(v, &v);
	BOOST_TEST(Vector3(-0.653f, -0.815f, 4.461f).equalTo(v, 1.0e-3));
}

/**
 * Tests construction of a matrix using rotation angles around the three major axes
 */
BOOST_AUTO_TEST_CASE(Matrix3_fromMajorAxisRotations)
{
	Matrix3 rot;
	Vector3 v;

	Matrix3::fromXRotation(deg2Rad(90.f), &rot);
	rot.multiply(Vector3(0.f, 0.f, 1.f), &v);
	BOOST_TEST(v.equalTo(Vector3(0.f, -1.f, 0.f), 1.0e-6));

	Matrix3::fromYRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(1.f, 0.f, 0.f), &v);
	BOOST_TEST(v.equalTo(Vector3(0.f, 0.f, -1.f), 1.0e-6));

	Matrix3::fromZRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(0.f, 1.f, 0.f), &v);
	BOOST_TEST(v.equalTo(Vector3(-1.f, 0.f, 0.f), 1.0e-6));
}

/**
 * Tests the method to check if a matrix is the identity matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_identity)
{
	float v[9] = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
	BOOST_TEST(Matrix3(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8])
		.isIdentity());

	for (int i = 0; i < 9; i++)
	{
		v[i] += 1;

		BOOST_TEST(!Matrix3(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]).isIdentity());

		v[i] -= 1;
	}
}

/**
 * Tests the method that compares two matrices for equality
 */
BOOST_AUTO_TEST_CASE(Matrix3_equalTo)
{
	float v[9] = { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f };
	Matrix3 m(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
	BOOST_TEST(m == m);

	Matrix3 m2;
	for (int i = 0; i < 9; i++)
	{
		v[i] += 0.1;
		m2 = Matrix3(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);

		// Strict comparison
		BOOST_TEST(m != m2);

		// Comparison w/ tolerance
		BOOST_TEST(m.equalTo(m2, 0.2f));

		v[i] -= 0.1;
	}
}

/**
 * Tests the ability to obtain the transpose of a matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_transpose)
{
	Matrix3 m(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f);
	m.transpose(&m);
	BOOST_TEST(m.equalTo(Matrix3(1.f, 4.f, 7.f, 2.f, 5.f, 8.f, 3.f, 6.f, 9.f)));
}

/**
 * Tests the ability to obtain the inverse of a matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_inverse)
{
	// Get inverse
	Matrix3 m(0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f);
	Matrix3 inv;
	bool hasInverse = m.inverse(&inv);
	BOOST_TEST(hasInverse);
	BOOST_TEST(inv.multiply(m).equalTo(Matrix3::IDENTITY, 1.0e-6));

	// Obtain cached value
	Matrix3 cached;
	hasInverse = m.inverse(&cached);
	BOOST_TEST(hasInverse);
	BOOST_TEST(cached == inv);

	// Inverse does not exist. Value remains unchanged.
	m = Matrix3(1.f, 2.f, 0.f, 3.f, -1.f, 2.f, -2.f, 3.f, -2.f);
	hasInverse = m.inverse(&inv);
	BOOST_TEST(!hasInverse);
	BOOST_TEST(inv == cached);

	// Obtain cached value (no inverse, so matrix is left unchanged)
	m.inverse(&cached);
	BOOST_TEST(!hasInverse);
	BOOST_TEST(cached == inv);
}

/**
 * Tests multiplication of a matrix with a column vector
 */
BOOST_AUTO_TEST_CASE(Matrix3_vectorMultiplication)
{
	Matrix3 m(
		1.5f, 2.0f, 3.1f,
		3.2f, 4.5f, 5.6f,
		6.1f, 7.2f, 8.3f);
	Vector3 v(2.1f, 3.0f, 4.2f);

	Vector3 expected(22.17f, 43.74f, 69.27f);

	Vector3 result;
	m.multiply(v, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}

/**
 * Tests multiplication of a matrix with another matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_matrixMultiplication)
{
	Matrix3 m1(
		1.5f, 2.0f, 3.1f,
		3.2f, 4.5f, 5.6f,
		6.1f, 7.2f, 8.3f);
	Matrix3 m2(
		2.1f, 1.3f, 3.4f,
		3.0f, 4.2f, 5.1f,
		6.2f, 7.4f, 8.5f);

	Matrix3 expected(
		28.37f, 33.29f, 41.65f,
		54.94f, 64.50f, 81.43f,
		85.87f, 99.59f, 128.01f);

	Matrix3 result;
	m1.multiply(m2, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}