#pragma once
#include <graphics/lights/pointers/AmbientLightPtr.h>
#include <graphics/lights/Light.h>

/**
 * A type of light that illuminates all parts of the scene equally
 * @author Nathaniel Rex
 */
class AmbientLight : public Light {

public:

	/**
	 * Constructs a new ambient light instance
	 */
	static AmbientLightPtr create();

protected:

	/**
	 * @override
	 * @throws UnsupportedOperationException When called, as changing position of ambient lighting is not supported.
	 */
	void updatePosition(float x, float y, float z) override;

	/**
	 * @override
	 * @throws UnsupportedOperationException When called, as rotation of ambient lighting is not supported.
	 */
	void updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) override;

private:

	/**
	 * Constructor
	 */
	AmbientLight();
};