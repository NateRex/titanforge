#pragma once
#include <graphics/core/shaders/pointers/PointShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Source code for the vertex shader used to handle point materials
 */
constexpr const char* POINT_VERTEX = R"(
    #version 330 core

    struct Transforms {
		mat4 model;
		mat4 view;
		mat4 proj;
	};

    layout (location = 0) in vec3 vert_Pos;

    uniform Transforms uTransforms;
    uniform float uSize;

    void main()
    {
        gl_Position = uTransforms.proj * uTransforms.view * uTransforms.model * vec4(vert_Pos, 1.0);
        gl_PointSize = uSize;
    }
)";

/**
 * Source code for the fragment shader used to handle point materials
 */
constexpr const char* POINT_FRAGMENT = R"(
    #version 330 core

    uniform vec4 uColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = uColor;
    }
)";


/**
 * Shader used to handle point primitives and their materials
 * @author Nathaniel Rex
 */
class PointShader : public Shader
{
public:

    /**
     * Creates a new PointShader instance
     * @return The PointShader instance
     */
    static PointShaderPtr create();

    void setItem(const RenderItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    PointShader();
};