#pragma once
#include <graphics/lights/pointers/DirectionalLightPtr.h>
#include <graphics/lights/Light.h>

class Vector3;

/**
 * A light that gets emitted in a specific direction. It will behave as though
 * it is infinitely far away and produce rays that are all parallel. The most common
 * use case for this is to simulate daylight.
 * @author Nathaniel Rex
 */
class DirectionalLight : public Light {

public:
    
    /**
     * Constructs a new directional light instance
     */
    static DirectionalLightPtr create();

    /**
     * Constructs a new directional light instance
     * @param direction Direction vector, in global space
     */
    static DirectionalLightPtr create(const Vector3& direction);

private:

    /**
     * Constructor
     * @param direction Starting direction vector, in global space
     */
    DirectionalLight(const Vector3& direction);
};