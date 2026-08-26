#include <graphics/core/shaders/PointShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/PointMaterial.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

PointShader::PointShader() : Shader("PointShader", POINT_VERTEX, POINT_FRAGMENT)
{

}

PointShaderPtr PointShader::create()
{
    return std::shared_ptr<PointShader>(new PointShader());
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
}