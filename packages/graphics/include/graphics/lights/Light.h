 #pragma once
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/lights/LightType.h>
#include <graphics/core/Entity.h>
#include <graphics/core/Color.h>

/**
 * A source of light that can be added to a scene. Lights take up no physical space, and instead affect the
 * shading of other entities.
 * @author Nathaniel Rex
 */
class Light : public Entity
{
public:

	/**
	 * Light type
	 */
	const LightType lightType;

	/**
	 * Color
	 */
	Color color;

	/**
	 * Intensity of the light, expressed as a value between 0 and 1. The greater the value, the greater the light
	 * intensity.
	 */
	float intensity;

protected:

	/**
	 * Constructor
	 * @param type
	 */
	Light(const LightType type) : Entity(EntityType::LIGHT), lightType(type), color(Color::WHITE), intensity(1.f) {};

	/**
	 * @override
	 * @throws UnsupportedOperationException When called, as scaling of lights is not supported.
	 */
	void updateScaling(float x, float y, float z) override;
};