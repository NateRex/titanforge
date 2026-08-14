#include <graphics/cameras/Camera.h>
#include <common/exceptions/UnsupportedOperationException.h>
#include <common/Utils.h>
#include <math.h>

Camera::Camera(): Entity(EntityType::CAMERA), _minPitch(-89.f), _maxPitch(89.f)
{
	// Camera starts at origin looking in -z direction
	lookAt(Vector3::ZERO, Vector3::MINUS_ZHAT, Vector3::YHAT);
}

Camera::~Camera()
{
	if (_pitch != nullptr)
	{
		delete _pitch;
	}

	_pitch = nullptr;
}

void Camera::setMinPitch(float min)
{
	_minPitch = min;
}

void Camera::setMaxPitch(float max)
{
	_maxPitch = max;
}

void Camera::addYaw(float degrees)
{
	addRotation(Matrix3::fromYRotation(deg2Rad(degrees)));
}

void Camera::addPitch(float degrees)
{
	// Compute pitch from starting orientation if this is the first time we are calling this method
	if (_pitch == nullptr)
	{
		Vector3 forward = getForwardVector();
		float pitch = rad2Deg(asin(forward.y));
		_pitch = new float(pitch);
	}

	// Clamp pitch to min and max values
	float clamped = clamp(*_pitch + degrees, _minPitch, _maxPitch);
	float delta = clamped - *_pitch;

	addRotation(Matrix3::fromXRotation(deg2Rad(delta)));
	*_pitch = clamped;
}

Matrix4 Camera::getViewMatrix()
{
	if (!_transformNeedsUpdate)
	{
		return _viewMatrix;
	}

	// Update local-to-world transform first
	updateTransform();

	// Invert to form view matrix
	Vector3 vT = _rotation.multiply(_position);
	_viewMatrix.setValues(
		_rotation[0], _rotation[1], _rotation[2], -vT.x,
		_rotation[3], _rotation[4], _rotation[5], -vT.y,
		-_rotation[6], -_rotation[7], -_rotation[8], vT.z,
		0.f, 0.f, 0.f, 1.f
	);

	return _viewMatrix;
}

void Camera::updateScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling not supported for cameras");
}