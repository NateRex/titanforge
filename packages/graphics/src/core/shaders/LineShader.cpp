#include <graphics/core/shaders/LineShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/LineMaterial.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

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
