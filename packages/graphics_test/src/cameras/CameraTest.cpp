#include <boost/test/unit_test.hpp>
#include <graphics/cameras/Camera.h>
#include <common/exceptions/UnsupportedOperationException.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>

/**
 * Example implementation of a Camera, used only for testing base class methods
 * @author Nathaniel Rex
 */
class TestCamera : public Camera
{
	Matrix4 getProjectionMatrix() const override
	{
		return Matrix4::IDENTITY;
	}
};

/**
 * Tests that trying to set the scaling of a camera results in exception
 */
BOOST_AUTO_TEST_CASE(Camera_scalingNotSupported)
{
	TestCamera camera;
	BOOST_REQUIRE_THROW(camera.setScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(camera.addScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
}

/**
 * Tests the ability to obtain the 3 axes of a camera
 */
BOOST_AUTO_TEST_CASE(Camera_axes)
{
	Vector3 v;

	// Camera starts off at origin, looking in -z direction
	TestCamera camera;
	BOOST_TEST(camera.getForwardVector(&v) == Vector3::MINUS_ZHAT);
	BOOST_TEST(camera.getUpVector(&v) == Vector3::YHAT);
	BOOST_TEST(camera.getRightVector(&v) == Vector3::XHAT);

	// Rotate each axis by 90 degrees
	camera.addRotation(Matrix3::fromXRotation(deg2Rad(90)));
	camera.addRotation(Matrix3::fromYRotation(deg2Rad(90)));
	camera.addRotation(Matrix3::fromZRotation(deg2Rad(90)));
	BOOST_TEST(camera.getForwardVector(&v).equalTo(Vector3::MINUS_XHAT, 1.0e-6));
	BOOST_TEST(camera.getUpVector(&v).equalTo(Vector3::YHAT, 1.0e-6));
	BOOST_TEST(camera.getRightVector(&v).equalTo(Vector3::MINUS_ZHAT, 1.0e-6));
}

/**
 * Tests that the view matrix is correctly computed as a result of manually setting the camera position and rotation.
 * Expected values were precomputed by hand.
 */
BOOST_AUTO_TEST_CASE(Camera_manual)
{
	TestCamera camera;
	Matrix4 exp;

	// Change yaw by 90 degrees
	camera.addRotation(Matrix3::fromYRotation(deg2Rad(90)));
	exp.setValues(0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-6));

	// Change pitch by 90 degrees
	camera.addRotation(Matrix3::fromXRotation(deg2Rad(90)));
	exp.setValues(0.f, 0.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-6));

	// Change roll by 90 degrees
	camera.addRotation(Matrix3::fromZRotation(deg2Rad(90)));
	exp.setValues(-1.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-6));

	// Change position
	camera.setPosition(10.f, 10.f, 10.f);
	exp.setValues(-1.f, 0.f, 0.f, 10.f, 0.f, 0.f, -1.f, 10.f, 0.f, -1.f, 0.f, 10.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-6));

}

/**
 * Tests that the view matrix is correctly computed as a result of calling the lookAt() method. Expected matrix
 * values were precomputed by hand.
 */
BOOST_AUTO_TEST_CASE(Camera_lookAt)
{
	TestCamera camera;
	Matrix4 exp;

	// Change target --------------------------------------
	Vector3 target(5.f, 0.f, 5.f);
	camera.lookAt(Vector3::ZERO, target, Vector3::YHAT);
	exp.setValues(-0.7071f, 0.f, 0.7071f, 0.f, 0.f, 1.f, 0.f, 0.f, -0.7071f, 0.f, -0.7071f, 0.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));

	// Change position ------------------------------------
	Vector3 position(1.f, 1.f, 1.f);
	camera.lookAt(position, target, Vector3::YHAT);
	exp.setValues(-0.7071f, 0.f, 0.7071f, 0.f, 0.1231f, 0.9840f, 0.1231f, -1.2302f, -0.6963f, 0.1741f, -0.6963f, 1.2185f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));

	// Change up vector ------------------------------------
	camera.lookAt(position, target, Vector3::XHAT);
	exp.setValues(0.f, 0.9701f, 0.2425f, -1.2126f, 0.7177f, 0.1688f, -0.6755f, -0.2109f, -0.6963, 0.1740f, -0.6963f, 1.2186f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));
}