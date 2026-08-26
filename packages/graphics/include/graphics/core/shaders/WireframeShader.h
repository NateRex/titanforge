#pragma once
#include <graphics/core/shaders/pointers/WireframeShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Source code for the vertex shader used to handle wireframe materials
 */
constexpr const char* WIREFRAME_VERTEX = R"(
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
constexpr const char* WIREFRAME_GEOMETRY = R"(
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
    uniform float uLineWidth;
    uniform vec2 uViewportSize;

    void emitLineVertex(int endpoint, float side, vec2 pixelNormal)
    {
        vec2 viewDirection = viewPosition[1].xy - viewPosition[0].xy;
        float directionLength = length(viewDirection);
        vec2 viewNormal = directionLength > 0.0
            ? vec2(-viewDirection.y, viewDirection.x) / directionLength
            : vec2(1.0, 0.0);
        vec3 expandedPosition = viewPosition[endpoint]
            + vec3(viewNormal * (side * uLineWidth * 0.5), 0.0);
        gl_Position = uTransforms.proj * vec4(expandedPosition, 1.0);
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
constexpr const char* WIREFRAME_FRAGMENT = R"(
    #version 330 core

    uniform vec4 uColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = uColor;
    }
)";

/**
 * Shader used to handle wireframe materials applied to meshes
 * @author Nathaniel Rex
 */
class WireframeShader : public Shader
{
    /**
     * Creates a new WireframeShader instance
     * @return The WireframeShader instance
     */
    static WireframeShaderPtr create()
    {
        return std::shared_ptr<WireframeShader>(new WireframeShader());
    }

    void setItem(const RenderItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    WireframeShader(): Shader("WireframeShader", WIREFRAME_VERTEX, WIREFRAME_GEOMETRY, WIREFRAME_FRAGMENT) {}
};