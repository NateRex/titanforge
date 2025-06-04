#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/entities/Entity.h>

/**
 * Parent class to all types of cameras that can be added to the scene.
 * @author Nathaniel Rex
 */
class Camera : public Entity
{
public:

	/**
	 * @override
	 * @throws UnsupportedOperationException When called, as scaling of cameras is not supported.
	 */
	void setScaling(float x, float y, float z) override;

	/**
	 * @override
	 * @throws UnsupportedOperationException When called, as scaling of cameras is not supported.
	 */
	void addScaling(float x, float y, float z) override;

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
	 * Constructor
	 */
	Camera();
};