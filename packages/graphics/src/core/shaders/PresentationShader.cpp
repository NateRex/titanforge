#include <graphics/core/shaders/PresentationShader.h>
#include <graphics/textures/Texture.h>
#include <glad/glad.h>

constexpr const char* PRESENTATION_FRAGMENT = R"(
    #version 330 core
    
    in vec2 frag_TexCoord;
    uniform sampler2D source;
    uniform float displayGamma;
    out vec4 FragColor;

    float adjust(float value) {
        if (value <= 0.0) return 0.0;
        if (value >= 1.0) return 1.0;
        
        // Equivalent to pow(value, 1 / displayGamma), without reciprocal overflow
        // or undefined pow(0, 0) for extreme but valid finite gamma values.
        return exp(log(value) / displayGamma);
    }

    void main() {
        vec4 value = texture(source, frag_TexCoord);
        vec3 adjusted = vec3(adjust(value.r), adjust(value.g), adjust(value.b));
        vec3 encoded = mix(1.055 * pow(adjusted, vec3(1.0 / 2.4)) - 0.055,
                            12.92 * adjusted, lessThanEqual(adjusted, vec3(0.0031308)));
        FragColor = vec4(encoded, value.a);
    }
)";

PresentationShader::PresentationShader() : FullScreenShader("PresentationShader", PRESENTATION_FRAGMENT) {}

void PresentationShader::draw(const TexturePtr& source, float displayGamma)
{
    activate();
    glUniform1i(getUniformLocation("source"), 0);
    glUniform1f(getUniformLocation("displayGamma"), displayGamma);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source->id());
    FullScreenShader::draw();
}
