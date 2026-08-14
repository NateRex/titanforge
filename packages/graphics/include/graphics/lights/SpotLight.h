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
     * Angle from the cone's center line, in degrees, within which the light is
     * at full intensity.
     */
    float innerAngle;

    /**
     * Angle from the cone's center line, in degrees, at which the light reaches
     * zero intensity. Intensity fades smoothly between innerAngle and this angle.
     */
    float outerAngle;

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
