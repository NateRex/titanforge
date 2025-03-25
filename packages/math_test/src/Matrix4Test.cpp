#include <boost/test/unit_test.hpp>
#include <math/Matrix4.h>
#include <math/Matrix3.h>
#include <math/Vector3.h>
#include <common/Constants.h>
#include <common/Utils.h>

/**
 * Tests the basic constructors and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Matrix4_basics)
{
	Matrix4 m;
	BOOST_TEST(m.isIdentity());

	m = Matrix4(
		1.f, 2.f, 3.f, 4.f,
		5.f, 6.f, 7.f, 8.f,
		9.f, 10.f, 11.f, 12.f,
		13.f, 14.f, 15.f, 16.f);
	for (int i = 0; i < 16; i++)
	{
		BOOST_TEST(m[i] == i + 1);
		BOOST_TEST(m.getValues()[i] == i + 1);
	}

	Matrix4 m2 = Matrix4(m);
	BOOST_TEST(m2 == m);
}

/**
 * Tests construction of scaling transformations
 */
BOOST_AUTO_TEST_CASE(Matrix4_scaling)
{
	Vector3 v(2.2f, -3.3f, 4.4f);
	Vector3 result;

	Matrix4 uniform = Matrix4::fromScaling(-5);
	uniform.transformDirection(v, &result);
	BOOST_TEST(result.x == v.x * -5);
	BOOST_TEST(result.y == v.y * -5);
	BOOST_TEST(result.z == v.z * -5);

	Matrix4 nonUniform = Matrix4::fromScaling(2.f, -3.f, 4.f);
	nonUniform.transformDirection(v, &result);
	BOOST_TEST(result.x == v.x * 2);
	BOOST_TEST(result.y == v.y * -3);
	BOOST_TEST(result.z == v.z * 4);
}

/**
 * Tests construction of a rotation transformation from a 3x3 matrix
 */
BOOST_AUTO_TEST_CASE(Matrix4_rotationFromMatrix)
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
	t.transformDirection(Vector3(1.f, 0.f, 0.f), &v);
	BOOST_TEST(v.equalTo(Vector3(0.f, 0.f, -1.f), 1.0e-6));
}

/**
 * Tests construction of rotation transformations using raw data values
 */
BOOST_AUTO_TEST_CASE(Matrix4_rotationFromValues)
{
	auto compare = [](const Matrix3& m1, const Matrix4& m2)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				BOOST_TEST(m1[i * 3 + j] == m2[i * 4 + j]);
			}
		}
	};

	Matrix3 rot = Matrix3::fromRotation(Vector3(0.662f, 0.2f, 0.722f), PI);
	Matrix4 trans = Matrix4::fromRotation(Vector3(0.662f, 0.2f, 0.722f), PI);
	compare(rot, trans);

	Matrix3::fromXRotation(PI, &rot);
	Matrix4::fromXRotation(PI, &trans);
	compare(rot, trans);

	Matrix3::fromYRotation(PI, &rot);
	Matrix4::fromYRotation(PI, &trans);
	compare(rot, trans);

	Matrix3::fromZRotation(PI, &rot);
	Matrix4::fromZRotation(PI, &trans);
	compare(rot, trans);
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
 * Tests construction of a matrix representing an orthographic projection
 */
BOOST_AUTO_TEST_CASE(Matrix4_orthographic)
{
	Matrix4 m = Matrix4::fromOrthographic(4, 3, 0.1, 100);

	float exp[16] = {
		0.5f, 0.f, 0.f, 0.f,
		0.f, 0.6667f, 0.f, 0.f,
		0.f, 0.f, -.02002f, -1.002f,
		0.f, 0.f, 0.f, 1.f
	};
	for (int i = 0; i < 16; i++)
	{
		BOOST_TEST(equals(m[i], exp[i], 1.0e-4));
	}

	Vector3 v(1.f, 1.f, 1.f);
	BOOST_TEST(m.transformPosition(v).equalTo(Vector3(0.5f, 0.6667f, -1.022f), 1.0e-4));
	BOOST_TEST(m.transformDirection(v).equalTo(Vector3(0.5, 0.6667, -0.02002), 1.0e-4));
}

/**
 * Tests construction of a matrix representing a perspective projection
 */
BOOST_AUTO_TEST_CASE(Matrix4_perspective)
{
	Matrix4 m = Matrix4::fromPerspective(deg2Rad(90.f), 16.f / 9.f, 0.1f, 100.f);
	
	float exp[16] = {
		0.5625f, 0.f, 0.f, 0.f,
		0.f, 1.0f, 0.f, 0.f,
		0.f, 0.f, -1.002f, -0.2002f,
		0.f, 0.f, -1.f, 0.f
	};
	for (int i = 0; i < 16; i++)
	{
		BOOST_TEST(equals(m[i], exp[i], 1.0e-4));
	}

	Vector3 v(1.f, 1.f, 1.f);
	BOOST_TEST(m.transformPosition(v).equalTo(Vector3(-0.5625f, -1.f, 1.2022f), 1.0e-4));
	BOOST_TEST(m.transformDirection(v).equalTo(Vector3(0.5625f, 1.f, -1.002f), 1.0e-4));
}

/**
 * Tests the method to check if a matrix is the identity matrix
 */
BOOST_AUTO_TEST_CASE(Matrix4_identity)
{
	float v[16] = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	Matrix4 m(
		v[0], v[1], v[2], v[3],
		v[4], v[5], v[6], v[7],
		v[8], v[9], v[10], v[11],
		v[12], v[13], v[14], v[15]);

	BOOST_TEST(m.isIdentity());

	for (int i = 0; i < 16; i++)
	{
		v[i] += 1;

		BOOST_TEST(!Matrix4(
			v[0], v[1], v[2], v[3],
			v[4], v[5], v[6], v[7],
			v[8], v[9], v[10], v[11],
			v[12], v[13], v[14], v[15]).isIdentity());

		v[i] -= 1;
	}
}

/**
 * Tests the method that compares two matrices for equality
 */
BOOST_AUTO_TEST_CASE(Matrix4_equalTo)
{
	float v[16] = {
		1.f, 2.f, 3.f, 4.f,
		5.f, 6.f, 7.f, 8.f,
		9.f, 10.f, 11.f, 12.f,
		13.f, 14.f, 15.f, 16.f
	};
	Matrix4 m(
		v[0], v[1], v[2], v[3],
		v[4], v[5], v[6], v[7],
		v[8], v[9], v[10], v[11],
		v[12], v[13], v[14], v[15]);

	BOOST_TEST(m == m);

	Matrix4 m2;
	for (int i = 0; i < 16; i++)
	{
		v[i] += 0.1;
		m2 = Matrix4(
			v[0], v[1], v[2], v[3],
			v[4], v[5], v[6], v[7],
			v[8], v[9], v[10], v[11],
			v[12], v[13], v[14], v[15]);

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
	Matrix4 m(
		1.f, 2.f, 3.f, 4.f,
		5.f, 6.f, 7.f, 8.f,
		9.f, 10.f, 11.f, 12.f,
		13.f, 14.f, 15.f, 16.f);
	m.transpose(&m);
	BOOST_TEST(m.equalTo(Matrix4(
		1.f, 5.f, 9.f, 13.f,
		2.f, 6.f, 10.f, 14.f,
		3.f, 7.f, 11.f, 15.f,
		4.f, 8.f, 12.f, 16.f)));
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
		2.f, 0.f, 0.f, 5.f,
		0.f, 3.f, 0.f, -4.f,
		0.f, 0.f, 1.f, 3.f,
		0.f, 0.f, 0.f, 1.f);
	Vector3 v(1., 2.f, 3.f);

	Vector3 expected(7.f, 2.f, 6.f);

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
		2.f, 0.f, 0.f, 5.f,
		0.f, 3.f, 0.f, -4.f,
		0.f, 0.f, 1.f, 3.f,
		0.f, 0.f, 0.f, 1.f);
	Vector3 v(1.f, 2.f, 3.f);

	Vector3 expected(2.f, 6.f, 3.f);

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
		2.f, 1.f, 3.f, 4.f,
		1.f, 2.f, 4.f, 3.f,
		3.f, 4.f, 1.f, 2.f,
		4.f, 3.f, 2.f, 1.f);
	Matrix4 m2(
		1.f, 3.f, 2.f, 4.f,
		4.f, 2.f, 1.f, 3.f,
		3.f, 1.f, 4.f, 2.f,
		2.f, 4.f, 3.f, 1.f);

	Matrix4 expected(
		23.f, 27.f, 29.f, 21.f,
		27.f, 23.f, 29.f, 21.f,
		26.f, 26.f, 20.f, 28.f,
		24.f, 24.f, 22.f, 30.f);

	Matrix4 result;
	m1.multiply(m2, &result);
	BOOST_TEST(result.equalTo(expected, 1.0e-3));
}