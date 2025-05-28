#include <graphics/cameras/PerspectiveCamera.h>
#include <common/Utils.h>

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
	: fov(fov), aspect(aspect), near(near), far(far)
{
	Matrix4::fromPerspective(deg2Rad(fov), aspect, near, far, &_projectionMatrix);
}

PerspectiveCameraPtr PerspectiveCamera::create(float fov, float aspect, float near, float far)
{
	return std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(fov, aspect, near, far));
}

Matrix4 PerspectiveCamera::getProjectionMatrix() const
{
	return _projectionMatrix;
}