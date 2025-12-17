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

private:

	/**
	 * Constructor
	 */
	AmbientLight() : Light(LightType::AMBIENT) {}
};