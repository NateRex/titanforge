#include <graphics/cameras/Camera.h>
#include <common/exceptions/UnsupportedOperationException.h>
#include <common/Utils.h>
#include <math.h>

Camera::Camera(): Entity(EntityType::CAMERA), _minPitch(-89.f), _maxPitch(89.f)
{
	// Camera starts at origin looking in -z direction
	lookAt(Vector3::ZERO, Vector3::MINUS_ZHAT, Vector3::YHAT);
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
	_yaw += degrees;
	rebuildRotation();
}

void Camera::addPitch(float degrees)
{
	// Clamp pitch to min and max values
	_pitch = clamp(_pitch + degrees, _minPitch, _maxPitch);
	rebuildRotation();
}

void Camera::rebuildRotation()
{
	const float yaw = deg2Rad(_yaw);
	const float pitch = deg2Rad(_pitch);
	const float cosYaw = cos(yaw);
	const float sinYaw = sin(yaw);
	const float cosPitch = cos(pitch);
	const float sinPitch = sin(pitch);

	// Rows store the camera's right, up, and forward axes. Yaw is measured around world Y and pitch around the
	// resulting right axis, so rebuilding these axes guarantees that no roll can accumulate.
	Entity::updateRotation(
		cosYaw, 0.f, -sinYaw,
		sinYaw * sinPitch, cosPitch, cosYaw * sinPitch,
		-sinYaw * cosPitch, sinPitch, -cosYaw * cosPitch
	);
}

void Camera::updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	Entity::updateRotation(m00, m01, m02, m10, m11, m12, m20, m21, m22);

	const Vector3 forward = getForwardVector();
	_pitch = rad2Deg(asin(clamp(forward.y, -1.f, 1.f)));
	_yaw = rad2Deg(atan2(-forward.x, -forward.z));
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
