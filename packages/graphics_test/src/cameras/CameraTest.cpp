#include <boost/test/unit_test.hpp>
#include <graphics/cameras/Camera.h>
#include <common/exceptions/UnsupportedOperationException.h>
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
 * Tests that the view matrix is correctly computed as a result of calling lookAt() methods
 */
BOOST_AUTO_TEST_CASE(Camera_lookAt)
{
	TestCamera camera;
	BOOST_TEST(camera.getViewMatrix() == Matrix4::IDENTITY);


}