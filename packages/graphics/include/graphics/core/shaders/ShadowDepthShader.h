#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

class Vector3;

/**
 * Shader used to perform depth-only mesh rendering from the perspective of a light source that casts shadows. This results in a shadow map
 * used during normal mesh rendering to produce shadow effects.
 * @author Nathaniel Rex
 */
class ShadowDepthShader : public Shader
{
public:

    /**
     * Constructs a new ShadowDepthShader instance
     * @return The new ShadowDepthShader instance
     */
    static ShaderPtr create() { return ShaderPtr(new ShadowDepthShader()); }

    /**
     * Upload one directional/spot light world-to-clip transform
     * @param transform Combined light projection and view matrix
     */
    void setProjection(const Matrix4& transform);

    /**
     * Upload six cubemap face transforms and normalized radial-depth parameters
     * @param transforms The combined projection and view matrix, for each of the six faces. These are expected in OpenGL cube face order:
     * +X, -X, +Y, -Y, +Z, -Z
     * @param position Light position
     * @param farPlane Far plane limiting how far away from the light we are drawing shadows
     */
    void setProjections(const Matrix4* transforms, const Vector3& position, float farPlane);

    void setItem(const DrawItem& item) override;

private:

    /**
     * Constructor
     */
    ShadowDepthShader();
};
