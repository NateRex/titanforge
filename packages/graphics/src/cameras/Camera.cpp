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
	// We use inverse direction of the camera to the target since we need to translate objects in the opposite direction
	Vector3 vForward = position.minus(target).normalize();
	Vector3 vUp = up.normalize();
	Vector3 vRight = up.cross(vForward).normalize();


}