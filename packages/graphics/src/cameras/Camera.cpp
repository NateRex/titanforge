#include <graphics/cameras/Camera.h>
#include <common/exceptions/UnsupportedOperationException.h>

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

void Camera::lookAt(const Vector3& target)
{
	lookAt(_position, target, Vector3::YHAT);
}

void Camera::lookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	Vector3 cDir = position.minus(target).normalize();
	Vector3 cRight = up.cross(cDir).normalize();
	Vector3 cUp = cDir.cross(cRight).normalize();

	// set local-to-world rotation
	setRotation(
		cRight.x,	cRight.y,	cRight.z,
		cUp.x,		cUp.y,		cUp.z,
		cDir.x,		cDir.y,		cDir.z
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
	Vector3 invTrans = _rotation.multiply(_position.scale(-1));
	_viewMatrix.setValues(
		_rotation[0], _rotation[1], _rotation[2], invTrans.x,
		_rotation[3], _rotation[4], _rotation[5], invTrans.y,
		_rotation[6], _rotation[7], _rotation[8], invTrans.z,
		0.f, 0.f, 0.f, 1.f
	);

	return _viewMatrix;
}