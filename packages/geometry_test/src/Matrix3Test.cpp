#include <boost/test/unit_test.hpp>
#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <common/Constants.h>

/**
 * Tests the basic constructors and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Matrix3_basics)
{
	Matrix3 m;
	BOOST_TEST(m.isIdentity());

	m = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
	for (int i = 0; i < 9; i++)
	{
		BOOST_TEST(m[i] == i + 1);
	}

	Matrix3 m2 = Matrix3(m);
	BOOST_TEST(m2 == m);
}

/**
 * Tests construction of a matrix using row and column vectors
 */
BOOST_AUTO_TEST_CASE(Matrix3_fromRowsAndColumns)
{
	Matrix3 expected = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);

	Matrix3 m;
	Matrix3::fromRows(Vector3(1, 2, 3), Vector3(4, 5, 6), Vector3(7, 8, 9), &m);
	BOOST_TEST(m == expected);

	Matrix3::fromColumns(Vector3(1, 4, 7), Vector3(2, 5, 8), Vector3(3, 6, 9), &m);
	BOOST_TEST(m == expected);
}

/**
 * Tests construction of a matrix using rotation angles around an arbitrary axis
 */
BOOST_AUTO_TEST_CASE(Matrix3_fromArbitraryAxisRotation)
{
	BOOST_TEST(Matrix3::fromRotation(Vector3(1., 0., 0.), PI) == Matrix3::fromXRotation(PI));
	BOOST_TEST(Matrix3::fromRotation(Vector3(0., 1., 0.), PI) == Matrix3::fromYRotation(PI));
	BOOST_TEST(Matrix3::fromRotation(Vector3(0., 0., 1.), PI) == Matrix3::fromZRotation(PI));

	Matrix3 rot = Matrix3::fromRotation(Vector3(0.662, 0.2, 0.722), deg2Rad(45));
	Vector3 v(-1., 2., 4.);

	rot.multiply(v, &v);
	BOOST_TEST(Vector3(-0.653, -0.815, 4.461).equalTo(v, 1.0e-3));
}

/**
 * Tests construction of a matrix using rotation angles around the three major axes
 */
BOOST_AUTO_TEST_CASE(Matrix3_fromMajorAxisRotations)
{
	Matrix3 rot;
	Vector3 v;

	Matrix3::fromXRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(0., 0., 1.), &v);
	BOOST_TEST(v.equalTo(Vector3(0., -1., 0.), 1.0e-6));

	Matrix3::fromYRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(1., 0., 0.), &v);
	BOOST_TEST(v.equalTo(Vector3(0., 0., -1.), 1.0e-6));

	Matrix3::fromZRotation(deg2Rad(90), &rot);
	rot.multiply(Vector3(0., 1., 0.), &v);
	BOOST_TEST(v.equalTo(Vector3(-1., 0., 0.), 1.0e-6));
}

/**
 * Tests the method to check if a matrix is the identity matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_identity)
{
	float v[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
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
	float v[9] = { 1, 2, 3, 4, 5, 6, 7, 8 };
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
		BOOST_TEST(m.equalTo(m2, 0.2));

		v[i] -= 0.1;
	}
}

/**
 * Tests the ability to obtain the transpose of a matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_transpose)
{
	Matrix3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);
	m.transpose(&m);
	BOOST_TEST(m.equalTo(Matrix3(1, 4, 7, 2, 5, 8, 3, 6, 9)));
}

/**
 * Tests the ability to obtain the inverse of a matrix
 */
BOOST_AUTO_TEST_CASE(Matrix3_inverse)
{
	// Get inverse
	Matrix3 m(0, 1, 0, 0, 0, 1, 1, 0, 0);
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
	m = Matrix3(1, 2, 0, 3, -1, 2, -2, 3, -2);
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
		1.5, 2.0, 3.1,
		3.2, 4.5, 5.6,
		6.1, 7.2, 8.3);
	Vector3 v(2.1, 3.0, 4.2);

	Vector3 expected(22.17, 43.74, 69.27);

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
		1.5, 2.0, 3.1,
		3.2, 4.5, 5.6,
		6.1, 7.2, 8.3);
	Matrix3 m2(
		2.1, 1.3, 3.4,
		3.0, 4.2, 5.1,
		6.2, 7.4, 8.5);

	Matrix3 expected(
		28.37, 33.29, 41.65,
		54.94, 64.50, 81.43,
		85.87, 99.59, 128.01);

	Matrix3 result;
	m1.multiply(m2, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}