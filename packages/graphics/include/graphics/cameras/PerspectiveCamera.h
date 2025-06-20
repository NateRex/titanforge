#pragma once
#include <graphics/cameras/pointers/PerspectiveCameraPtr.h>
#include <graphics/cameras/Camera.h>

/**
 * Camera that uses a perspective projection. This projection mode is designed to mimic the way the human eye sees. It
 * is the most common projection mode used for rendering a 3D scene.
 * @author Nathaniel Rex
 */
class PerspectiveCamera : public Camera
{
public:

	/**
	 * Constructs a new PerspectiveCamera instance
	 * @param fov Field of view angle (in degrees)
	 * @param aspect Aspect ratio
	 * @param near Near plane distance
	 * @param far Far plane distance
	 */
	static PerspectiveCameraPtr create(float fov, float aspect, float near, float far);

	/**
	 * @return The current field of view angle (in degrees)
	 */
	float getFOV() const { return fov; }

	/**
	 * Set the field of view angle
	 * @param value Field of view angle (in degrees)
	 */
	void setFOV(float value);

	/**
	 * @return The current aspect ratio
	 */
	float getAspectRatio() const { return aspect; }

	/**
	 * Set the aspect ratio
	 * @param value Aspect ratio
	 */
	void setAspect(float value);

	/**
	 * @return The current near plane distance from the camera
	 */
	float getNearDistance() const { return near; }

	/**
	 * Set the near plane distance from the camera
	 * @param value Near plane distance
	 */
	void setNearDistance(float value);

	/**
	 * @return The current far plane distance from the camera
	 */
	float getFarDistance() const { return far; }

	/**
	 * Set the far plane distance from the camera
	 * @param value Far plane distance
	 */
	void setFarDistance(float value);

	/**
	 * @override
	 */
	Matrix4 getProjectionMatrix() const override;

private:

	/**
	 * Field of view angle (in degrees)
	 */
	float fov;

	/**
	 * Aspect ratio
	 */
	float aspect;

	/**
	 * Near plane distance
	 */
	float near;

	/**
	 * Far plane distance
	 */
	float far;

	/**
	 * Projection matrix
	 */
	Matrix4 _projectionMatrix;

	/**
	 * Constructor
	 * @param fov Field of view angle (in degrees)
	 * @param aspect Aspect ratio
	 * @param near Near plane distance
	 * @param far Far plane distance
	 */
	PerspectiveCamera(float fov, float aspect, float near, float far);
};