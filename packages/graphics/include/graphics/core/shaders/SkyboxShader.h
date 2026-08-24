#pragma once
#include <graphics/core/shaders/pointers/SkyboxShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Source code for the vertex shader used to handle skyboxes
 */
constexpr const char* SKYBOX_VERTEX = R"(
    #version 330 core

    layout (location = 0) in vec3 vert_Pos;

    uniform mat4 uView;
    uniform mat4 uProjection;

    out vec3 frag_TexCoord;

    void main()
    {
        // Skybox is passed to us as a box geometry. Convert to clip space
        // (ignoring translation, so that skybox is always visible)
        frag_TexCoord = vert_Pos;
        vec4 clip = uProjection * mat4(mat3(uView)) * vec4(position, 1.0);
        gl_Position = uProjection * uView * vec4(vert_Pos, 1.0);
    }
)";

/**
 * Source code for the fragment shader used to handle skyboxes
 */
constexpr const char* SKYBOX_FRAGMENT = R"(
    #version 330 core

    in vec3 frag_TexCoord;
    
    uniform samplerCube uTexture;
    uniform vec4 uColor;
    uniform float uIntensity;
    uniform float uRotation;
    uniform float uLod;

    out vec4 FragColor;

    void main()
    {
        // Apply rotation
        float c = cos(uRotation);
        float s = sin(uRotation);
        vec3 direction = normalize(frag_TexCoord);
        direction.xz = mat2(c, -s, s, c) * direction.xz;

        // Sample and generate color
        vec4 sampleColor = textureLod(uTexture, direction, max(uLod, 0.0));
        FragColor = vec4(sampleColor.rgb * uColor.rgb * max(uIntensity, 0.0),
                         sampleColor.a * uColor.a);
    }
)";

/**
 * Shader used to handle skyboxes
 * @author Nathaniel Rex
 */
class SkyboxShader : public Shader
{
public:

    /**
	 * Constructs a new SkyboxShader instance. This should typically only be done once, by the shader manager.
	 * @return The new SkyboxShader instance
	 */
    static SkyboxShaderPtr create();

    void setCamera(const CameraPtr camera) override;

    void setMaterial(const MaterialPtr material) override;

private:

    /**
     * Constructor
     */
    SkyboxShader();
};
