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

void Camera::lookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	Vector3 vDirection = position.minus(target).normalize();
	Vector3 vUp = up.normalize();
	Vector3 vRight = up.cross(vDirection).normalize();

	// local-to-world rotation
	setRotation(
		vRight.x, vUp.x, vDirection.x,
		vRight.y, vUp.y, vDirection.y,
		vRight.z, vUp.z, vDirection.z
	);

	// local-to-world translation
	setPosition(position.x, position.y, position.z);
}