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
	BOOST_TEST(v.equalTo(Vector3(origin.x - 10, origin.y + 20, origin.y - 30)));
}