#include <boost/test/unit_test.hpp>
#include <geometry/Matrix4.h>
#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>

/**
 * Tests the basic constructors and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Matrix4_basics)
{
	Matrix4 m;
	BOOST_TEST(m.isIdentity());

	m = Matrix4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	for (int i = 0; i < 16; i++)
	{
		BOOST_TEST(m[i] == i + 1);
	}

	Matrix4 m2 = Matrix4(m);
	BOOST_TEST(m2 == m);
}

/**
 * Tests construction of scaling transformations
 */
BOOST_AUTO_TEST_CASE(Matrix4_scaling)
{
	Vector3 v(2.2, -3.3, 4.4);
	Vector3 result;

	Matrix4 uniform = Matrix4::fromScaling(-5);
	uniform.transformDirection(v, &result);
	BOOST_TEST(result.x == v.x * -5);
	BOOST_TEST(result.y == v.y * -5);
	BOOST_TEST(result.z == v.z * -5);

	Matrix4 nonUniform = Matrix4::fromScaling(2, -3, 4);
	nonUniform.transformDirection(v, &result);
	BOOST_TEST(result.x == v.x * 2);
	BOOST_TEST(result.y == v.y * -3);
	BOOST_TEST(result.z == v.z * 4);
}

/**
 * Tests construction of a rotation transformation
 */
BOOST_AUTO_TEST_CASE(Matrix4_rotation)
{
	Matrix3 rot;
	Matrix4 t;
	Vector3 v;

	Matrix3::fromXRotation(deg2Rad(90), &rot);
	Matrix4::fromRotation(rot, &t);
	t.transformDirection(Vector3(0, 0, 1), &v);
	BOOST_TEST(v.equalTo(Vector3(0, -1, 0), 1.0e-6));

	Matrix3::fromYRotation(deg2Rad(90), &rot);
	Matrix4::fromRotation(rot, &t);
	t.transformDirection(Vector3(1, 0, 0), &v);
	BOOST_TEST(v.equalTo(Vector3(0, 0, -1), 1.0e-6));
}

/**
 * Tests construction of a translation transformation
 */
BOOST_AUTO_TEST_CASE(Matrix4_translation)
{
	Vector3 v;
	Vector3 origin(1, 2, 3);

	Matrix4 t = Matrix4::fromTranslation(Vector3(-10, 20, -30));
	t.transformDirection(origin, &v);		// no effect
	BOOST_TEST(v.equalTo(origin));

	t.transformPosition(origin, &v);		// translation
	BOOST_TEST(v.equalTo(Vector3(origin.x - 10, origin.y + 20, origin.z - 30)));
}

/**
 * Tests the method to check if a matrix is the identity matrix
 */
BOOST_AUTO_TEST_CASE(Matrix4_identity)
{
	float v[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	Matrix4 m(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
	BOOST_TEST(m.isIdentity());

	for (int i = 0; i < 16; i++)
	{
		v[i] += 1;

		BOOST_TEST(!Matrix4(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15])
			.isIdentity());

		v[i] -= 1;
	}
}

/**
 * Tests the method that compares two matrices for equality
 */
BOOST_AUTO_TEST_CASE(Matrix4_equalTo)
{
	float v[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	Matrix4 m(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
	BOOST_TEST(m == m);

	Matrix4 m2;
	for (int i = 0; i < 16; i++)
	{
		v[i] += 0.1;
		m2 = Matrix4(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);

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
BOOST_AUTO_TEST_CASE(Matrix4_transpose)
{
	Matrix4 m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	m.transpose(&m);
	BOOST_TEST(m.equalTo(Matrix4(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16)));
}

/**
 * Tests the ability to obtain the inverse of a matrix
 */
BOOST_AUTO_TEST_CASE(Matrix4_inverse)
{
	Matrix4 m(0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
	Matrix4 inv;
	bool hasInverse = m.inverse(&inv);
	BOOST_TEST(hasInverse);
	BOOST_TEST(inv.multiply(m).equalTo(Matrix4::IDENTITY, 1.0e-6));

	// Obtain cached value
	Matrix4 cached;
	hasInverse = m.inverse(&cached);
	BOOST_TEST(hasInverse);
	BOOST_TEST(cached == inv);

	// Inverse does not exist. Value remains unchanged.
	m = Matrix4(1, 2, 3, 4, 2, 4, 6, 8, 3, 6, 9, 12, 4, 8, 12, 16);
	hasInverse = m.inverse(&inv);
	BOOST_TEST(!hasInverse);
	BOOST_TEST(inv == cached);

	// Obtain cached value (no inverse, so matrix is left unchanged)
	m.inverse(&cached);
	BOOST_TEST(!hasInverse);
	BOOST_TEST(cached == inv);
}

/**
 * Tests the ability to transform a position vector
 */
BOOST_AUTO_TEST_CASE(Matrix4_transformPosition)
{
	Matrix4 m(
		2., 0., 0., 5.,
		0., 3., 0., -4.,
		0., 0., 1., 3.,
		0., 0., 0., 1.);
	Vector3 v(1., 2., 3.);

	Vector3 expected(7., 2., 6.);

	Vector3 result;
	m.transformPosition(v, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}

/**
 * Tests the ability to transform a direction vector
 */
BOOST_AUTO_TEST_CASE(Matrix4_transformDirection)
{
	Matrix4 m(
		2., 0., 0., 5.,
		0., 3., 0., -4.,
		0., 0., 1., 3.,
		0., 0., 0., 1.);
	Vector3 v(1., 2., 3.);

	Vector3 expected(2., 6., 3.);

	Vector3 result;
	m.transformDirection(v, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}

/**
 * Tests multiplication of a matrix with another matrix
 */
BOOST_AUTO_TEST_CASE(Matrix4_matrixMultiplication)
{
	Matrix4 m1(
		2., 1., 3., 4.,
		1., 2., 4., 3.,
		3., 4., 1., 2.,
		4., 3., 2., 1.);
	Matrix4 m2(
		1., 3., 2., 4.,
		4., 2., 1., 3.,
		3., 1., 4., 2.,
		2., 4., 3., 1.);

	Matrix4 expected(
		23., 27., 29., 21.,
		27., 23., 29., 21.,
		26., 26., 20., 28.,
		24., 24., 22., 30.);

	Matrix4 result;
	m1.multiply(m2, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}