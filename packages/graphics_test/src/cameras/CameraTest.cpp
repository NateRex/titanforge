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
 * Tests that the view matrix is correctly computed as a result of manually setting the camera position and rotation.
 * Expected values were precomputed by hand.
 */
BOOST_AUTO_TEST_CASE(Camera_manual)
{
	TestCamera camera;
	BOOST_TEST(camera.getViewMatrix() == Matrix4::IDENTITY);

	Matrix4 exp;

	// Change rotation
	camera.setRotation(Matrix3::fromRotation(Vector3::YHAT, deg2Rad(135.f)));
	exp.setValues(-0.7071f, 0.f, 0.7071f, 0.f, 0.f, 1.f, 0.f, 0.f, -0.7071f, 0.f, -0.7071f, 0.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));

	// Change position
	camera.setPosition(1.f, 1.f, 1.f);
	exp.setValues(-0.7071f, 0.f, 0.7071f, 0.f, 0.f, 1.f, 0.f, -1.f, -0.7071f, 0.f, -0.7071f, 1.4142f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));
}

/**
 * Tests that the view matrix is correctly computed as a result of calling the lookAt() method. Expected
 * values were precomputed by hand.
 */
BOOST_AUTO_TEST_CASE(Camera_lookAt)
{
	TestCamera camera;
	BOOST_TEST(camera.getViewMatrix() == Matrix4::IDENTITY);

	Matrix4 exp;

	// Change target
	Vector3 target(5.f, 0.f, 5.f);
	camera.lookAt(Vector3::ZERO, target, Vector3::YHAT);
	exp.setValues(-0.7071f, 0.f, 0.7071f, 0.f, 0.f, 1.f, 0.f, 0.f, -0.7071f, 0.f, -0.7071f, 0.f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));

	// Change position
	Vector3 position(1.f, 1.f, 1.f);
	camera.lookAt(position, target, Vector3::YHAT);
	exp.setValues(-0.7071f, 0.f, 0.7071f, 0.f, 0.1231f, 0.9840f, 0.1231f, -1.2302f, -0.6963f, 0.1741f, -0.6963f, 1.2185f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));

	// Change up vector
	camera.lookAt(position, target, Vector3::XHAT);
	exp.setValues(0.f, 0.9701f, 0.2425f, -1.2126f, 0.7177f, 0.1688f, -0.6755f, -0.2109f, -0.6963, 0.1740f, -0.6963f, 1.2186f, 0.f, 0.f, 0.f, 1.f);
	BOOST_TEST(camera.getViewMatrix().equalTo(exp, 1.0e-3));
}