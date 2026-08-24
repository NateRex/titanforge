#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/core/Entity.h>

/**
 * Parent class to all types of cameras that can be added to the scene.
 * @author Nathaniel Rex
 */
class Camera : public Entity
{
public:

	/**
	 * @return A matrix capable of transforming objects from world space to view space relative to this camera
	 */
	Matrix4 getViewMatrix();

	/**
	 * @return A matrix capable of transforming objects from view space to projection (clip) space relative to this
	 * camera
	 */
	virtual Matrix4 getProjectionMatrix() const = 0;

	/**
	 * Sets the lower bound for the pitch angle of this camera's orientation
	 * @param min Lower bound on the pitch angle, in degrees
	 */
	void setMinPitch(float min);

	/**
	 * Sets the upper bound for the pitch angle of this camera's orientation
	 * @param max Upper bound on the pitch angle, in degrees
	 */
	void setMaxPitch(float max);

	/**
	 * Applies a yaw rotation to this camera's orientation around the global up axis
	 * @param degrees The yaw angle (in degrees) to rotate the camera by
	 */
	void addYaw(float degrees);

	/**
	 * Applies a pitch rotation to this camera's orientation relative to a horizon defined by the camera's starting orientation.
	 * The resulting value will be clamped to the lower and upper limits defined via Camera::setMinPitch and Camera::setMaxPitch.
	 * @param degrees The pitch angle (in degrees) to rotate the camera by
	 */
	void addPitch(float degrees);

protected:

	/**
	 * Matrix representing the transformation from world to view space, relative to this camera. This is the inverse
	 * of the camera's local-to-world transformation matrix.
	 */
	Matrix4 _viewMatrix;

	/**
	 * The lower bound on the pitch angle of this camera's orientation, in degrees.
	 */
	float _minPitch;

	/**
	 * The upper bound on the pitch angle of this camera's orientation, in degrees.
	 */
	float _maxPitch;

	/**
	 * The yaw angle represented by the camera's current orientation, in degrees. By storing this value, it is easier
	 * to apply rotation changes around a horizon-locked orientation, instead of accumulating rotations around axes that have
	 * already been tilted.
	 */
	float _yaw = 0.f;

	/**
	 * The pitch angle represented by the camera's current orientation, in degrees. By storing this value, it is easier
	 * to apply rotation changes around a horizon-locked orientation, instead of accumulating rotations around axes that have
	 * already been tilted.
	 */
	float _pitch = 0.f;

	/**
	 * Constructor
	 */
	Camera();

	/**
	 * Rebuilds the camera basis for the current yaw and pitch
	 */
	void rebuildRotation();

	/**
	 * @copydoc Entity::updateScaling(float, float, float)
	 * @throws UnsupportedOperationException When called, as scaling of cameras is not supported.
	 */
	void updateScaling(float x, float y, float z) override;

	void updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) override;
};
