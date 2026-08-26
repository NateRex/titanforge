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

    out vec3 viewPosition;

    void main()
    {
        vec4 viewPos = uTransforms.view * uTransforms.model * vec4(vert_Pos, 1.0);
        viewPosition = viewPos.xyz;
        gl_Position = uTransforms.proj * viewPos;
    }
)";

/**
 * Source code for the geometry shader used to expand lines to screen-facing quads
 */
constexpr const char* LINE_GEOMETRY = R"(
    #version 330 core

    struct Transforms {
        mat4 model;
        mat4 view;
        mat4 proj;
    };

    layout (lines) in;
    layout (triangle_strip, max_vertices = 4) out;

    in vec3 viewPosition[];

    uniform Transforms uTransforms;
    uniform float uWidth;
    uniform vec2 uViewportSize;
    uniform bool uUseWorldWidth;

    void emitLineVertex(int endpoint, float side, vec2 pixelNormal)
    {
        if (uUseWorldWidth)
        {
            vec2 viewDirection = viewPosition[1].xy - viewPosition[0].xy;
            float directionLength = length(viewDirection);
            vec2 viewNormal = directionLength > 0.0
                ? vec2(-viewDirection.y, viewDirection.x) / directionLength
                : vec2(1.0, 0.0);
            vec3 expandedPosition = viewPosition[endpoint]
                + vec3(viewNormal * (side * uWidth * 0.5), 0.0);
            gl_Position = uTransforms.proj * vec4(expandedPosition, 1.0);
        }
        else
        {
            gl_Position = gl_in[endpoint].gl_Position;
            gl_Position.xy += pixelNormal * side * uWidth / uViewportSize * gl_Position.w;
        }
        EmitVertex();
    }

    void main()
    {
        vec2 ndc0 = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
        vec2 ndc1 = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
        vec2 pixelDirection = (ndc1 - ndc0) * uViewportSize;
        float directionLength = length(pixelDirection);
        vec2 pixelNormal = directionLength > 0.0
            ? vec2(-pixelDirection.y, pixelDirection.x) / directionLength
            : vec2(1.0, 0.0);

        emitLineVertex(0, -1.0, pixelNormal);
        emitLineVertex(0,  1.0, pixelNormal);
        emitLineVertex(1, -1.0, pixelNormal);
        emitLineVertex(1,  1.0, pixelNormal);
        EndPrimitive();
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
    LineShader(): Shader("LineShader", LINE_VERTEX, LINE_GEOMETRY, LINE_FRAGMENT) {}
};
