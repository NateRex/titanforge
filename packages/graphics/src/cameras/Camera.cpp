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
	Vector3 vDirection = position.minus(target).normalize();
	Vector3 vUp = up.normalize();
	Vector3 vRight = up.cross(vDirection).normalize();

	// set local-to-world rotation
	setRotation(
		vRight.x, vUp.x, vDirection.x,
		vRight.y, vUp.y, vDirection.y,
		vRight.z, vUp.z, vDirection.z
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
	Matrix3 invRot = _rotation.transpose();
	Vector3 invTrans = _rotation.multiply(_position.scale(-1));
	_viewMatrix.setValues(
		invRot[0], invRot[1], invRot[2], invTrans.x,
		invRot[3], invRot[4], invRot[5], invTrans.y,
		invRot[6], invRot[7], invRot[8], invTrans.z,
		0.f, 0.f, 0.f, 1.f
	);

	return _viewMatrix;
}