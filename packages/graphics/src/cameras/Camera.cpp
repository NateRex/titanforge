#include <graphics/cameras/Camera.h>
#include <common/exceptions/UnsupportedOperationException.h>
#include <common/Utils.h>

Camera::Camera(): Entity(EntityType::CAMERA)
{

}

void Camera::setScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling not supported for cameras");
}

void Camera::addScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling not supported for cameras");
}

Vector3 Camera::getRightVector(Vector3* result) const
{
	Vector3& r = getOrDefault(result, Vector3());
	_rotation.getRow(0, &r);
	return r.normalize(&r);
}

Vector3 Camera::getUpVector(Vector3* result) const
{
	Vector3& r = getOrDefault(result, Vector3());
	_rotation.getRow(1, &r);
	return r.normalize(&r);
}

Vector3 Camera::getForwardVector(Vector3* result) const
{
	Vector3& r = getOrDefault(result, Vector3());
	_rotation.getRow(2, &r);
	return r.normalize(&r);
}

void Camera::lookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	Vector3 f = target.minus(position).normalize();
	Vector3 r = f.cross(up).normalize();
	Vector3 u = r.cross(f).normalize();

	// set local-to-world rotation
	setRotation(
		r.x,	r.y,	r.z,
		u.x,	u.y,	u.z,
		-f.x,	-f.y,	-f.z
	);

	// local-to-world translation
	setPosition(position.x, position.y, position.z);
}

Matrix4 Camera::getViewMatrix()
{
	if (!_transformNeedsUpdate)
	{
		return _viewMatrix;
	}

	// Update local-to-world transform first
	updateTransform();

	// Since cameras contain affine transformations (rotation and translation only), we
	// can optimize how we compute the inverse.
	Vector3 invTrans = _rotation.multiply(_position);
	_viewMatrix.setValues(
		_rotation[0], _rotation[1], _rotation[2], -invTrans.x,
		_rotation[3], _rotation[4], _rotation[5], -invTrans.y,
		_rotation[6], _rotation[7], _rotation[8], -invTrans.z,
		0.f, 0.f, 0.f, 1.f
	);

	return _viewMatrix;
}