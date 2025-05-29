#include <boost/test/unit_test.hpp>
#include <graphics/cameras/PerspectiveCamera.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>

/**
 * Tests basic construction and field assignment
 */
BOOST_AUTO_TEST_CASE(PerspectiveCamera_basics)
{
	float aspect = 800.f / 600.f;
	PerspectiveCameraPtr camera = PerspectiveCamera::create(30.f, aspect, 0.1f, 100.f);
	BOOST_TEST(camera->fov == 30.f);
	BOOST_TEST(camera->aspect == aspect);
	BOOST_TEST(camera->near == 0.1f);
	BOOST_TEST(camera->far == 100.f);
}

/**
 * Tests that the projection matrix for a perspective camera is computed correctly
 */
BOOST_AUTO_TEST_CASE(PerspectiveCamera_projectionMatrix)
{
	float fov = 45.f;
	float aspect = 800.f / 600.f;
	float near = 0.1f;
	float far = 100.f;
	CameraPtr camera = PerspectiveCamera::create(fov, aspect, near, far);
	BOOST_TEST(camera->getProjectionMatrix() == Matrix4::fromPerspective(deg2Rad(fov), aspect, near, far));
}