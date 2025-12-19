#include <graphics/core/shaders/BasicShader.h>
#include <graphics/materials/BasicMaterial.h>
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

	BasicMaterialPtr mat = cast<BasicMaterial>(material);

	// Texture
	if (mat->texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mat->texture->id());
		glUniform1i(getUniformLocation("uMaterial.texture"), GL_TEXTURE0);
		glUniform1i(getUniformLocation("uMaterial.hasTexture"), 1);
	}

	// Vertex color usage
	glUniform1i(getUniformLocation("uMaterial.hasVertexColor"), mat->useVertexColors ? 1 : 0);
}