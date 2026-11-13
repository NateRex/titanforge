#pragma once
#include <graphics/core/renderer/pointers/RenderTargetPtr.h>
#include <vector>

class Light;
class RenderTarget;
class ShadowDepthShader;
class Matrix4;
class Vector3;
struct DrawState;
struct LightInstance;

/**
 * Hardware capacity available to a shadow pass, shared by all lights in a frame
 * @author Nathaniel Rex
 */
struct ShadowHardwareLimits
{

    /**
     * Number of texture units available for shadow maps after reserving units for material and environment textures.
     */
    unsigned int shadowSlots;

    /**
     * Maximum 2D shadow-map width or height in pixels, limited by texture storage and viewport dimensions.
     */
    int max2DSize;

    /**
     * Maximum shadow-cubemap face size in pixels, limited by cubemap storage and viewport dimensions.
     */
    int maxCubeSize;
};

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
     * Reusable depth-only render targets, indexed by successful shadow selection order. Targets
     * belong to slots rather than individual lights, so a target may be reused for a different light on a subsequent
     * frame.
     */
    std::vector<RenderTargetPtr> _targets;

    /**
     * @return The sampler capacity and texture dimensions supported by the current context
     */
    static ShadowHardwareLimits queryHardwareLimits();

    /**
     * Reuses a compatible cached target or allocates one. Cache publication follows successful rendering.
     * @param index Shadow map index
     * @param light The light we are using to produce shadows
     * @return A render target, either newly-allocated or repurposed from the cache. This value should be cached after successful
     * rendering.
     */
    RenderTargetPtr prepareTarget(unsigned int index, const Light& light) const;

    /**
     * Binds and clears the target, configures the light projection, and submits shadow casters to render depth onto the target.
     * @param state Draw state
     * @param light The light we are using to produce shadows
     * @param transforms World-to-light clip-space transform matrices. Only one matrix is expected when producing a 2D shadow map.
     * Six matrices are required to produce a layered cube map.
     * @param target Render target
     */
    static void renderDepth(const DrawState& state, const LightInstance& light,
        const Matrix4* transforms, const RenderTarget& target);

    /**
     * Submits elligible meshes to the shader, rendering depth onto the current target
     * @param state Draw state
     * @param shader Shadow depth shader
     */
    static void renderCasters(const DrawState& state, ShadowDepthShader& shader);

    /**
     * Validates a light's shadow projection settings and builds the world-to-light clip-space transformation
     * matrices.
     * 
     * Directional lights use an orthographic projection centered on a configured directional volume. Spotlights
     * use a perspective projection derived from their outer cone half-angle, with the light's world space position
     * as the viewpoint. Point lights use a perspective projection to each face of a cube map.
     * 
     * @param light Light instance
     * @param maxSize Max supported width and height of the shadow map texture, in pixels
     * @param transforms Output storage to hold one matrix for 2D shadows, or six matrices for point-light cubemaps.
     * Results are unspecified if this method returns false.
     * @return True if the settings were found to be valid and the resulting transformation matrix was computed. Returns
     * false otherwise.
     */
    static bool buildTransforms(const LightInstance& light, int maxSize, Matrix4* transforms);

    /**
     * Builds a shadow transform using position, forward, and up vectors.
     * @param position Point origin
     * @param forward Forward vector
     * @param up Up vector
     * @return The resulting transformation matrix
     */
    static Matrix4 buildTransform(const Vector3& position, const Vector3& forward, const Vector3& up);
};