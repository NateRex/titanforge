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
	 * Non-negative brightness multiplier for the light. A value of 0 disables the light's contribution completely.
	 * A value of 1 uses the baseline brightness. Values greater than 1 make it proportionally brighter. For example,
	 * a value of 2 produces twice the radiance of 1 before attenuation and other lighting terms are applied.
	 * Negative values are treated as 0 by the renderer.
	 */
	float intensity;

	/**
	 * Whether the light becomes dimmer with distance.
	 */
	bool attenuation;

	/**
	 * Approximate distance at which an attenuated light contributes 1% of its
	 * original brightness. Must be greater than zero when attenuation is enabled.
	 */
	float range;

	void traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

	/**
	 * Constructor
	 * @param type
	 */
	Light(const LightType type) :
		lightType(type),
		color(Color::WHITE),
		intensity(1.f),
		attenuation(true),
		range(100.f)
	{};

	/**
	 * @copydoc Entity::updateScaling(float, float, float)
	 * @throws UnsupportedOperationException When called, as scaling of lights is not supported.
	 */
	void updateScaling(float x, float y, float z) override;
};
