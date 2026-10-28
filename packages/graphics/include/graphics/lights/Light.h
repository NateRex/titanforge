#pragma once
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/lights/LightType.h>
#include <graphics/core/Entity.h>
#include <graphics/core/Color.h>

/**
 * Distance falloff for point and spot lights when Light::attenuation is enabled.
 * @author Nathaniel Rex
 */
enum class AttenuationModel
{
	/**
	 * 1 / max(distance, minDistance)^2, in world units
	 */
	INVERSE_SQUARE,

	/**
	 * Range-based curve using the max distance defined by Light::range
	 */
	RANGE_BASED

};

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
	 * Linear RGB light color with unencoded alpha. Defaults to white.
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
	 * Whether point and spot lights fade with distance. Defaults to true.
	 */
	bool attenuation;

	/**
	 * Point and spot light distance model. Ignored when attenuation is false. Defaults to INVERSE_SQUARE.
	 */
	AttenuationModel attenuationModel;

	/**
	 * Approximate falloff distance (in world units) for RANGE_BASED attenuation. Defaults to 100.
	 * This setting has no effect on inverse-square attenuation.
	 */
	float range;

	/**
	 * The inverse-square distance clamp (in world units) for INVERSE_SQUARE attenuation. Defaults to 0.01.
	 * This setting has no effect on range-based attenuation.
	 */
	float minimumDistance;

	void traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

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
		attenuationModel(AttenuationModel::INVERSE_SQUARE),
		range(100.f),
		minimumDistance(0.01f)
	{};

	/**
	 * @copydoc Entity::updateScaling(float, float, float)
	 * @throws UnsupportedOperationException When called, as scaling of lights is not supported.
	 */
	void updateScaling(float x, float y, float z) override;
};
