#pragma once
#include <graphics/core/shaders/FullScreenShader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <memory>

/**
 * Presents linear RGB using display adjustment and standard sRGB encoding, preserving alpha.
 * Owns its fullscreen triangle. The renderer configures the destination and disables blending
 * and framebuffer sRGB encoding before drawing. Instances belong to one OpenGL context.
 */
class PresentationShader : public FullScreenShader
{
public:

    /**
     * Constructs a new PresentationShader instance
     * @return The new PresentationShader instance
     */
    static ShaderPtr create()
    {
        return std::shared_ptr<PresentationShader>(new PresentationShader());
    }

    /**
     * Draws a linear input texture with RGB clamped to [0,1], display adjustment, then sRGB encoding.
     * @param source Linear scene texture after postprocessing.
     * @param displayGamma Finite positive display adjustment exponent, validated by Renderer.
     */
    void draw(const TexturePtr& source, float displayGamma);

private:

    /**
     * Constructor
     */
    PresentationShader();
};
