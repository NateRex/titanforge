#pragma once
#include <graphics/core/renderer/pointers/RenderTargetPtr.h>
#include <set>
#include <string>
#include <vector>

struct DrawState;

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
};