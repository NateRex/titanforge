#include <graphics/core/shaders/BasicShader.h>
#include <graphics/materials/Material.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

BasicShader::BasicShader() : Shader("BasicShader", BASIC_VERTEX, BASIC_FRAGMENT)
{

}

BasicShaderPtr BasicShader::create()
{
	return std::shared_ptr<BasicShader>(new BasicShader());
}

void BasicShader::setMaterial(const MaterialPtr material)
{
	Shader::setMaterial(material);

	// Texture
	if (mat->texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mat->texture->id());
		glUniform1i(getUniformLocation("uMaterial.texture"), 0);
		glUniform1i(getUniformLocation("uMaterial.hasTexture"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasTexture"), 0);
	}

	// Diffuse map
	if (mat->diffuseMap)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mat->diffuseMap->id());
		glUniform1i(getUniformLocation("uMaterial.diffuseMap"), 1);
		glUniform1i(getUniformLocation("uMaterial.hasDiffuseMap"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasDiffuseMap"), 0);
	}

	// Specular map
	if (mat->specularMap)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mat->specularMap->id());
		glUniform1i(getUniformLocation("uMaterial.specularMap"), 2);
		glUniform1i(getUniformLocation("uMaterial.hasSpecularMap"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasSpecularMap"), 0);
	}

	// Vertex color usage
	glUniform1i(getUniformLocation("uMaterial.hasVertexColor"), mat->useVertexColors ? 1 : 0);
}
