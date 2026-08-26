#include <graphics/core/shaders/SkyboxShader.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/cameras/Camera.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <algorithm>

SkyboxShader::SkyboxShader() : Shader("SkyboxShader", SKYBOX_VERTEX, SKYBOX_FRAGMENT) {}

SkyboxShaderPtr SkyboxShader::create()
{
    return std::shared_ptr<SkyboxShader>(new SkyboxShader());
}

void SkyboxShader::setCamera(Camera* camera)
{
    if (!camera) throw IllegalArgumentException("SkyboxShader requires a camera");
    ProgramBinding binding(this);
    glUniformMatrix4fv(getUniformLocation("uView"), 1, GL_TRUE, camera->getViewMatrix().getValues());
    glUniformMatrix4fv(getUniformLocation("uProjection"), 1, GL_TRUE, camera->getProjectionMatrix().getValues());
}

void SkyboxShader::setMaterial(const Material* material)
{
    if (!material || material->materialType != MaterialType::SKYBOX)
        throw IllegalArgumentException("SkyboxShader requires a SkyboxMaterial");

    const SkyboxMaterial* skyboxMat = static_cast<const SkyboxMaterial*>(material);

    if (!skyboxMat->texture)
	{
		throw IllegalArgumentException("SkyboxMaterial requires an input cube texture");
	}

    const TextureCubePtr skyboxTexture = std::static_pointer_cast<TextureCube>(skyboxMat->texture);
    if (!skyboxTexture)
    {
        throw IllegalArgumentException("SkyboxMaterial texture must be a cube texture");
    }

    ProgramBinding binding(this);
    Shader::setMaterial(material);

    // Cube map texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture->id());
    glUniform1i(getUniformLocation("uTexture"), 0);

    // Color, intensity, rotation, and LOD uniforms
    const Color color = skyboxMat->color;
    glUniform4f(getUniformLocation("uColor"), color.red(), color.green(), color.blue(), color.alpha());
    glUniform1f(getUniformLocation("uIntensity"), std::max(skyboxMat->intensity, 0.f));
    glUniform1f(getUniformLocation("uRotation"), skyboxMat->rotation);
    glUniform1f(getUniformLocation("uLod"), std::max(skyboxMat->lod, 0.f));

    // Disable face culling
    glDisable(GL_CULL_FACE);
}
