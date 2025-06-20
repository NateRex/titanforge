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

void PerspectiveCamera::setFOV(float value)
{
	fov = value;
	Matrix4::fromPerspective(deg2Rad(fov), aspect, near, far, &_projectionMatrix);
}

void PerspectiveCamera::setAspect(float value)
{
	aspect = value;
	Matrix4::fromPerspective(deg2Rad(fov), aspect, near, far, &_projectionMatrix);
}

void PerspectiveCamera::setNearDistance(float value)
{
	near = value;
	Matrix4::fromPerspective(deg2Rad(fov), aspect, near, far, &_projectionMatrix);
}

void PerspectiveCamera::setFarDistance(float value)
{
	far = value;
	Matrix4::fromPerspective(deg2Rad(fov), aspect, near, far, &_projectionMatrix);
}

Matrix4 PerspectiveCamera::getProjectionMatrix() const
{
	return _projectionMatrix;
}