#include <graphics/core/shaders/PointShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/PointMaterial.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>


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
    uniform float uViewportHeight;
    uniform bool uUseWorldSize;

    void main()
    {
        gl_Position = uTransforms.proj * uTransforms.view * uTransforms.model * vec4(vert_Pos, 1.0);

        // If using world sizing, convert world-space diameter to pixels using the vertical projection scale,
        // perspective divide, and current viewport height.
        gl_PointSize = uUseWorldSize
            ? uSize * uTransforms.proj[1][1] * uViewportHeight / (2.0 * gl_Position.w)
            : uSize;
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

PointShader::PointShader(): Shader("PointShader", POINT_VERTEX, POINT_FRAGMENT)
{

}

void PointShader::setItem(const RenderItem& item)
{
    Shader::setItem(item);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.model"), 1, GL_TRUE, item.modelTransform.getValues());
}
    
void PointShader::setCamera(Camera* camera)
{
    Shader::setCamera(camera);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.view"), 1, GL_TRUE, camera->getViewMatrix().getValues());
    glUniformMatrix4fv(getUniformLocation("uTransforms.proj"), 1, GL_TRUE, camera->getProjectionMatrix().getValues());

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glUniform1f(getUniformLocation("uViewportHeight"), viewport[3]);
}

void PointShader::setMaterial(const Material* material)
{
    if (!material || material->materialType != MaterialType::POINT)
    {
        throw IllegalArgumentException("PointShader requires a PointMaterial");
    }

    Shader::setMaterial(material);
    ProgramBinding binding(this);
    const PointMaterial* pointMat = static_cast<const PointMaterial*>(material);
    
    // Color
    Color color = pointMat->color;
    glUniform4f(getUniformLocation("uColor"), color.red(), color.green(), color.blue(), color.alpha());

    // Size
    glUniform1f(getUniformLocation("uSize"), pointMat->size);
    glUniform1i(getUniformLocation("uUseWorldSize"), pointMat->sizeUnits == PrimitiveSizeUnits::WORLD ? 1 : 0);
}
