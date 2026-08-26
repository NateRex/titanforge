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

    layout (triangles) in;
    layout (triangle_strip, max_vertices = 12) out;

    in vec3 viewPosition[];

    uniform Transforms uTransforms;
    uniform float uLineWidth;
    
    void emitLineVertex(int endpoint, float side, vec2 viewNormal)
    {
        vec3 expandedPosition = viewPosition[endpoint]
            + vec3(viewNormal * (side * uLineWidth * 0.5), 0.0);
        gl_Position = uTransforms.proj * vec4(expandedPosition, 1.0);
        EmitVertex();
    }

    void emitEdge(int start, int end)
    {
        vec2 viewDirection = viewPosition[end].xy - viewPosition[start].xy;
        float directionLength = length(viewDirection);
        vec2 viewNormal = directionLength > 0.0
            ? vec2(-viewDirection.y, viewDirection.x) / directionLength
            : vec2(1.0, 0.0);

        emitLineVertex(start, -1.0, viewNormal);
        emitLineVertex(start,  1.0, viewNormal);
        emitLineVertex(end,   -1.0, viewNormal);
        emitLineVertex(end,    1.0, viewNormal);
        EndPrimitive();
    }

    void main()
    {
        emitEdge(0, 1);
        emitEdge(1, 2);
        emitEdge(2, 0);
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
public:

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
