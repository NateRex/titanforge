#include <graphics/core/shaders/LineShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/LineMaterial.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>


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

LineShader::LineShader() : Shader("LineShader", LINE_VERTEX, LINE_GEOMETRY, LINE_FRAGMENT)
{
    
}

void LineShader::setItem(const RenderItem& item)
{
    Shader::setItem(item);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.model"), 1, GL_TRUE, item.modelTransform.getValues());
}
    
void LineShader::setCamera(Camera* camera)
{
    Shader::setCamera(camera);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.view"), 1, GL_TRUE, camera->getViewMatrix().getValues());
    glUniformMatrix4fv(getUniformLocation("uTransforms.proj"), 1, GL_TRUE, camera->getProjectionMatrix().getValues());

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glUniform2f(getUniformLocation("uViewportSize"), viewport[2], viewport[3]);
}

void LineShader::setMaterial(const Material* material)
{
    if (!material || material->materialType != MaterialType::LINE)
    {
        throw IllegalArgumentException("LineShader requires a LineMaterial");
    }

    Shader::setMaterial(material);
    ProgramBinding binding(this);
    const LineMaterial* lineMat = static_cast<const LineMaterial*>(material);
    
    // Color
    Color color = lineMat->color;
    glUniform4f(getUniformLocation("uColor"), color.red(), color.green(), color.blue(), color.alpha());

    // Size
    glUniform1f(getUniformLocation("uWidth"), lineMat->width);
    glUniform1i(getUniformLocation("uUseWorldWidth"), lineMat->widthUnits == PrimitiveSizeUnits::WORLD ? 1 : 0);
}
