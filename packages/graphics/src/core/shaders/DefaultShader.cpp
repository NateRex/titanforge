#include <graphics/core/shaders/DefaultShader.h>
#include <graphics/materials/Material.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

DefaultShader::DefaultShader() : Shader("DefaultShader", DEFAULT_VERTEX, DEFAULT_FRAGMENT)
{

}

DefaultShaderPtr DefaultShader::create()
{
	return std::shared_ptr<DefaultShader>(new DefaultShader());
}

void DefaultShader::setMaterial(const MaterialPtr material)
{
	Shader::setMaterial(material);

	// Texture
	if (material->texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->texture->id());
		glUniform1i(getUniformLocation("uMaterial.texture"), 0);
		glUniform1i(getUniformLocation("uMaterial.hasTexture"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasTexture"), 0);
	}

	// Diffuse map
	if (material->diffuseMap)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->diffuseMap->id());
		glUniform1i(getUniformLocation("uMaterial.diffuseMap"), 1);
		glUniform1i(getUniformLocation("uMaterial.hasDiffuseMap"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasDiffuseMap"), 0);
	}

	// Specular map
	if (material->specularMap)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->specularMap->id());
		glUniform1i(getUniformLocation("uMaterial.specularMap"), 2);
		glUniform1i(getUniformLocation("uMaterial.hasSpecularMap"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasSpecularMap"), 0);
	}

	// Vertex color usage
	glUniform1i(getUniformLocation("uMaterial.hasVertexColor"), material->useVertexColors ? 1 : 0);
}
