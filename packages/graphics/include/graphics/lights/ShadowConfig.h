#pragma once
#include <math/Vector3.h>

/**
 * Shadow settings shared by all lights capable of producing shadows
 * @author Nathaniel Rex
 */
struct ShadowConfig
{

    /**
     * Boolean flag that, when true, indicates that the light is capable of producing shadows
     */
    bool enabled = true;

    /**
     * Pixel width of the texture used to store the shadow map
     */
    int width = 1024;

    /**
     * Pixel height of the texture used to store the shadow map
     */
    int height = 1024;

    /**
     * Distance, in world units, from the light to its near clipping plane. Any shadows closer to the light will be clipped.
     */
    float near = 0.1f;

    /**
     * Distance, in world units, from the light to the far clipping plane. Any shadows further from the light will be clipped.
     */
    float far = 100.f;

    /**
     * Shadow settings specific to directional lights. Controls world-space coverage perpendicular to a directional light's rays.
     */
    struct DirectionalVolume
    {
        /**
         * The world-space center of the shadow volume. Defaults to the world origin.
         */
        Vector3 center = Vector3::ZERO;

        /**
         * Width of the shadow volume, in world units, measured along the light's right axis. Extends equally on either side of the volume
         * center. Defaults to 20.
         */
        float width = 20.f;

        /**
         * Height of the shadow volume, in world units, measured along the light's up axis. Extends equally above and below the volume center.
         * Defaults to 20.
         */
        float height = 20.f;
    } directional;
};
