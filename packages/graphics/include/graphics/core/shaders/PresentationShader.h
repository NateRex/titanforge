#pragma once
#include <graphics/core/shaders/FullScreenShader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <memory>
#include <graphics/core/ToneMapping.h>

/**
 * Presents linear HDR RGB using exposure, tone mapping, display adjustment, and standard sRGB encoding.
 * Preserves source alpha.
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
     * Draws a linear input texture: remove negative RGB, apply exposure and the selected tone curve,
     * clamp to [0,1], apply display adjustment, then encode sRGB exactly once. Alpha is unchanged.
     * @param source Linear scene texture after postprocessing.
     * @param displayGamma Finite positive display adjustment exponent, validated by Renderer.
     * @param toneMapping Recognized tone curve, validated by Renderer.
     * @param exposure Finite nonnegative final brightness multiplier, validated by Renderer.
     */
    void draw(const TexturePtr& source, float displayGamma, ToneMapping toneMapping, float exposure);

private:

    /**
     * Constructor
     */
    PresentationShader();
};
