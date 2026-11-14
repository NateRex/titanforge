#include <graphics/core/shaders/PresentationShader.h>
#include <graphics/textures/Texture.h>
#include <glad/glad.h>

constexpr const char* PRESENTATION_FRAGMENT = R"(
    #version 330 core
    
    in vec2 frag_TexCoord;
    uniform sampler2D source;
    uniform float displayGamma;
    uniform int toneMapping;
    uniform float exposure;
    out vec4 FragColor;

    // Values shared with ToneMapping.
    const int REINHARD = 1;
    const int EXPONENTIAL = 2;

    float adjust(float value) {
        if (value <= 0.0) return 0.0;
        if (value >= 1.0) return 1.0;
        
        // Equivalent to pow(value, 1 / displayGamma), without reciprocal overflow
        // or undefined pow(0, 0) for extreme but valid finite gamma values.
        return exp(log(value) / displayGamma);
    }

    float mapChannel(float value) {
        // Handle zero first, including HDR values that have overflowed their storage.
        if (exposure == 0.0 || value <= 0.0) return 0.0;
        // All curves round to display white at this scale. Saturate before multiplication
        // can overflow, so no curve depends on GLSL built-ins accepting infinity.
        if (value > 1.0 && exposure > 1.0e20 / value) return 1.0;
        float x = value * exposure;
        // Stable equivalent of x / (1 + x), without overflowing 1 + x.
        if (toneMapping == REINHARD) return x <= 1.0 ? x / (1.0 + x) : 1.0 / (1.0 + 1.0 / x);
        if (toneMapping == EXPONENTIAL) return 1.0 - exp(-x);
        return x;
    }

    void main() {
        vec4 value = texture(source, frag_TexCoord);
        vec3 mapped = clamp(vec3(mapChannel(value.r), mapChannel(value.g), mapChannel(value.b)), 0.0, 1.0);
        vec3 adjusted = vec3(adjust(mapped.r), adjust(mapped.g), adjust(mapped.b));
        vec3 encoded = mix(1.055 * pow(adjusted, vec3(1.0 / 2.4)) - 0.055,
                            12.92 * adjusted, lessThanEqual(adjusted, vec3(0.0031308)));
        FragColor = vec4(encoded, value.a);
    }
)";

PresentationShader::PresentationShader() : FullScreenShader("PresentationShader", PRESENTATION_FRAGMENT) {}

void PresentationShader::draw(const TexturePtr& source, float displayGamma, ToneMapping toneMapping, float exposure)
{
    activate();
    glUniform1i(getUniformLocation("source"), 0);
    glUniform1f(getUniformLocation("displayGamma"), displayGamma);
    glUniform1i(getUniformLocation("toneMapping"), static_cast<int>(toneMapping));
    glUniform1f(getUniformLocation("exposure"), exposure);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source->id());
    FullScreenShader::draw();
}
