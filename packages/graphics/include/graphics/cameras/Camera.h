#pragma once
#include <graphics/entities/Entity.h>

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
	virtual Matrix4 getViewMatrix() const = 0;

	/**
	 * @return A matrix capable of transforming objects from view space to projection (clip) space relative to this
	 * camera
	 */
	virtual Matrix4 getProjectionMatrix() const = 0;

	/**
	 * Rotates this camera in-place to look at a given target
	 * @param target Target world position
	 */
	void lookAt(const Vector3& target);

	/**
	 * Rotates and moves the camera to look at a given target
	 * @param position New camera world position
	 * @param target Target world position
	 * @param up Vector representing the "up" direction of the camera, in world space
	 */
	void lookAt(const Vector3& position, const Vector3& target, const Vector3& up);

private:

	/**
	 * Constructor
	 */
	Camera();
};