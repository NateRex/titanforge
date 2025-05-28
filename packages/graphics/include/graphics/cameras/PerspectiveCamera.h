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
	 * Field of view angle (in degrees)
	 */
	const float fov;

	/**
	 * Aspect ratio
	 */
	const float aspect;

	/**
	 * Near plane distance
	 */
	const float near;

	/**
	 * Far plane distance
	 */
	const float far;

	/**
	 * Constructs a new PerspectiveCamera instance
	 * @param fov Field of view angle (in degrees)
	 * @param aspect Aspect ratio
	 * @param near Near plane distance
	 * @param far Far plane distance
	 */
	static PerspectiveCameraPtr create(float fov, float aspect, float near, float far);

	/**
	 * @override
	 */
	Matrix4 getProjectionMatrix() const override;

private:

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