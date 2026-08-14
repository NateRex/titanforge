#pragma once
#include <graphics/lights/pointers/PointLightPtr.h>
#include <graphics/lights/Light.h>

/**
 * A light that illuminates in all directions from a point in space
 * @author Nathaniel Rex
 */
class PointLight : public Light {

public:

	/**
	 * Whether the light becomes dimmer with distance.
	 */
	bool attenuation;

	/**
	 * Approximate distance at which an attenuated light contributes 1% of its
	 * original brightness. Must be greater than zero when attenuation is enabled.
	 */
	float range;

	/**
	 * Constructs a new point light instance
	 */
	static PointLightPtr create();

private:

	/**
	 * Constructor
	 */
	PointLight() : Light(LightType::POINT), attenuation(true), range(50.f) {}
};
