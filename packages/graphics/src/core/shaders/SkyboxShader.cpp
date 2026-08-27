#include <graphics/core/shaders/SkyboxShader.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/cameras/Camera.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <algorithm>

/**
 * Source code for the vertex shader used to handle skyboxes
 */
constexpr const char* SKYBOX_VERTEX = R"(
    #version 330 core

    layout (location = 0) in vec3 vert_Pos;

    uniform mat4 uView;
    uniform mat4 uProjection;

    out vec3 frag_TexCoord;

    void main()
    {
        // Skybox is passed to us as a box geometry with coordinates spanning [-1, 1]. Convert to clip space,
        // and then assign the z component equal to the w component, so that we will always end up with a
        // depth buffer value of 1.
        frag_TexCoord = vert_Pos;
        vec4 position = uProjection * mat4(mat3(uView)) * vec4(vert_Pos, 1.0);
        gl_Position = position.xyww;
    }
)";

/**
 * Source code for the fragment shader used to handle skyboxes
 */
constexpr const char* SKYBOX_FRAGMENT = R"(
    #version 330 core

    in vec3 frag_TexCoord;
    
    uniform samplerCube uTexture;
    uniform vec4 uColor;
    uniform float uIntensity;
    uniform float uRotation;
    uniform float uLod;

    out vec4 FragColor;

    void main()
    {
        // Apply rotation
        float c = cos(uRotation);
        float s = sin(uRotation);
        vec3 direction = normalize(frag_TexCoord);
        direction.xz = mat2(c, -s, s, c) * direction.xz;

        // Sample and generate color
        vec4 sampleColor = textureLod(uTexture, direction, max(uLod, 0.0));
        FragColor = vec4(sampleColor.rgb * uColor.rgb * max(uIntensity, 0.0),
                         sampleColor.a * uColor.a);
    }
)";

SkyboxShader::SkyboxShader() : Shader("SkyboxShader", SKYBOX_VERTEX, SKYBOX_FRAGMENT)
{

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
}
