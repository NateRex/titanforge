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
        frag_TexCoord = vert_Pos;
        gl_Position = uProjection * uView * vec4(vert_Pos, 1.0);
    }
)";

/**
 * Source code for the fragment shader used to handle skyboxes
 */
constexpr const char* SKYBOX_FRAGMENT = R"(
    #version 330 core

    in vec3 frag_TexCoord;

    uniform samplerCube uSkyBox;

    out vec4 FragColor;

    void main()
    {    
        FragColor = texture(uSkyBox, TexCoords);
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