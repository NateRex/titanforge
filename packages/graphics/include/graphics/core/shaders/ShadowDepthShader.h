#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

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
     * Set the projection matrix
     * @param transform Projection transformation matrix
     */
    void setProjection(const Matrix4& transform);

    void setItem(const DrawItem& item) override;

private:

    /**
     * Constructor
     */
    ShadowDepthShader();
};
