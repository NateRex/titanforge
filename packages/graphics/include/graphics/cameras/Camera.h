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
	 * Destructor
	 */
	~Camera();

	/**
	 * Computes the unit vector representing the "up" direction of the camera, in global space
	 * @param result (Optional) Vector in which to store the result
	 * @return The up vector
	 */
	Vector3 getUpVector(Vector3* result = nullptr) const;

	/**
	 * Computes the unit vector representing the "forward" direction of the camera, in global space
	 * @param result (Optional) Vector in which to store the result
	 * @return The forward vector
	 */
	Vector3 getForwardVector(Vector3* result = nullptr) const;

	/**
	 * Computes the unit vector representing the "right" direction of the camera, in global space
	 * @param result (Optional) Vector in which to store the result
	 * @return The right vector
	 */
	Vector3 getRightVector(Vector3* result = nullptr) const;

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
	 * Applies a yaw rotation to this camera's orientation.
	 * @param degrees The yaw angle (in degrees) to rotate the camera by
	 */
	void addYaw(float degrees);

	/**
	 * Applies a pitch rotation to this camera's orientation. This method additionally ensures the resulting value
	 * is clamped to the lower and upper limits on pitch. These limits can be changed via setMinPitch() and setMaxPitch().
	 * @param degrees The pitch angle (in degrees) to rotate the camera by
	 */
	void addPitch(float degrees);

	/**
	 * Rotates and moves the camera to look at a given target
	 * @param position New camera world position
	 * @param target Target world position
	 * @param up Vector representing the "up" direction of the camera, in world space
	 */
	void lookAt(const Vector3& position, const Vector3& target, const Vector3& up);

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
	 * The current pitch angle of the camera's orientation. Is lazy-initialized the first time addPitch() is called,
	 * at which point the value is computed from the camera's starting orientation.
	 */
	float* _pitch = nullptr;

	/**
	 * Constructor
	 */
	Camera();

	/**
	 * @override
	 * @throws UnsupportedOperationException When called, as scaling of cameras is not supported.
	 */
	void updateScaling(float x, float y, float z) override;
};