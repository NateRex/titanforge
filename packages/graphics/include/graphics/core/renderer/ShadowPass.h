#pragma once
#include <graphics/core/renderer/pointers/RenderTargetPtr.h>
#include <set>
#include <string>
#include <vector>

class Matrix4;
struct DrawState;
struct LightInstance;

/**
 * A shadow pass renders shadow maps to offscreen targets, and prepares light instances for shadow
 * sampling during material rendering.
 * 
 * A shadow pass is configured with depth-only render targets reused between frames. Elligible lights
 * are selected in scene traversal order to cast shadows, subject to the requested budget, available
 * texture units, and shader capacity.
 * 
 * Ambient lights do not cast shadows. Mesh participation follows material settings.
 * 
 * @author Nathaniel Rex
 */
class ShadowPass
{
public:

    /**
     * Renders depth maps for elligible lights and stores the resulting shadow texture, world-to-light
     * clip transform, and sampler index on each succesfully shadowed light instance.
     * @param state Current frame's traversed scene data
     * @param budget Maximum number of shadow maps to render. A value of 0 causes no maps to be rendered,
     * and thus disables shadows.
     */
    void render(DrawState& state, unsigned int budget);

    /**
     * Releases this pass's ownership of all cached shadow render targets.
     */
    void clear();

private:

    /**
     * Reusable depth-only render targets, indexed by the shadow sampler slot assigned to a light for the
     * current frame. Targets belong to slots rather than individual lights, so a target may be reused for
     * a different light on a subsequent frame.
     */
    std::vector<RenderTargetPtr> _targets;

    /**
     * Validates a light's shadow projection settings and builds the world-to-light clip-space transformation
     * matrix.
     * 
     * Directional lights use an orthographic projection centered on a configured directional volume. Spotlights
     * use a perspective projection derived from their outer cone half-angle, with the light's world space position
     * as the viewpoint.
     * 
     * @param light Light instance
     * @param maxSize Max supported width and height of the shadow map texture, in pixels
     * @param transform Matrix to hold the resulting transform. Value will be modified in-place. Result is unspecified
     * if this method returns a value of false.
     * @return True if the settings were found to be valid and the resulting transformation matrix was computed. Returns
     * false otherwise.
     */
    static bool buildTransform(const LightInstance& light, int maxSize, Matrix4& transform);
};