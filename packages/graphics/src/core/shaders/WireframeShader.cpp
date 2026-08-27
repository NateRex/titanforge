#include <graphics/core/shaders/WireframeShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/WireframeMaterial.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

void WireframeShader::setItem(const RenderItem& item)
{
    Shader::setItem(item);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.model"), 1, GL_TRUE, item.modelTransform.getValues());
}
    
void WireframeShader::setCamera(Camera* camera)
{
    Shader::setCamera(camera);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.view"), 1, GL_TRUE, camera->getViewMatrix().getValues());
    glUniformMatrix4fv(getUniformLocation("uTransforms.proj"), 1, GL_TRUE, camera->getProjectionMatrix().getValues());

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glUniform2f(getUniformLocation("uViewportSize"), viewport[2], viewport[3]);
}

void WireframeShader::setMaterial(const Material* material)
{
    if (!material || material->materialType != MaterialType::WIREFRAME)
    {
        throw IllegalArgumentException("WireframeShader requires a WireframeMaterial");
    }

    Shader::setMaterial(material);
    ProgramBinding binding(this);
    const WireframeMaterial* wfMat = static_cast<const WireframeMaterial*>(material);
    
    // Color
    Color color = wfMat->color;
    glUniform4f(getUniformLocation("uColor"), color.red(), color.green(), color.blue(), color.alpha());

    // Size
    glUniform1f(getUniformLocation("uLineWidth"), wfMat->lineWidth);
    glUniform1i(getUniformLocation("uUseWorldLineWidth"),
        wfMat->lineWidthUnits == PrimitiveSizeUnits::WORLD ? 1 : 0);
}

