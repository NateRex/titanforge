#pragma once
#include <graphics/core/shaders/pointers/LineShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Source code for the vertex shader used to handle line materials
 */
constexpr const char* LINE_VERTEX = R"(
    #version 330 core

    struct Transforms {
		mat4 model;
		mat4 view;
		mat4 proj;
	};

    layout (location = 0) in vec3 vert_Pos;

    uniform Transforms uTransforms;
    uniform float uWidth;
    uniform float uViewportHeight;
    uniform bool uUseWorldWidth;

    void main()
    {
        gl_Position = uTransforms.proj * uTransforms.view * uTransforms.model * vec4(vert_Pos, 1.0);

        // If using world sizing, convert world-space diameter to pixels using the vertical projection scale,
        // perspective divide, and current viewport height.
        gl_PointSize = uUseWorldWidth
            ? uWidth * uTransforms.proj[1][1] * uViewportHeight / (2.0 * gl_Position.w)
            : uWidth;
    }
)";

/**
 * Source code for the fragment shader used to handle line materials
 */
constexpr const char* LINE_FRAGMENT = R"(
    #version 330 core

    uniform vec4 uColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = uColor;
    }
)";

/**
 * Shader used to handle lines and their materials
 * @author Nathaniel Rex
 */
class LineShader : public Shader
{
public:

    /**
     * Creates a new LineShader instance
     * @return The LineShader instance
     */
    static LineShaderPtr create()
    {
        return std::shared_ptr<LineShader>(new LineShader());
    }

    void setItem(const RenderItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    LineShader(): Shader("LineShader", LINE_VERTEX, LINE_FRAGMENT) {}
};
