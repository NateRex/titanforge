#pragma once
#include <graphics/lights/pointers/SpotLightPtr.h>
#include <graphics/lights/Light.h>

/**
 * A light that gets emitted from a single point in one direction along a
 * cone.
 * @author Nathaniel Rex
 */
class SpotLight : public Light {

public:

    /**
     * The angle of the cone formed by this light.
     */
    float angle;

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
     * Constructs a new spotlight instance
     */
    static SpotLightPtr create();

    /**
     * Constructs a new spotlight instance
     * @param position Starting position of this light
     * @param direction Direction the light is pointing
     */
    static SpotLightPtr create(const Vector3& position, const Vector3& direction);

private:

    /**
     * Constructor
     * @param position Starting position of this light
     * @param direction Direction the light is pointing
     */
    SpotLight(const Vector3& position, const Vector3& direction);
};