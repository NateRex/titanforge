#pragma once
#include <graphics/lights/pointers/PointLightPtr.h>
#include <graphics/lights/Light.h>

/**
 * A light that illuminates from a point in space
 * @author Nathaniel Rex
 */
class PointLight : public Light {

public:

	/**
	 * Constructs a new point light instance
	 */
	static PointLightPtr create();

private:

	/**
	 * Constructor
	 */
	PointLight() : Light(LightType::POINT) {}
};