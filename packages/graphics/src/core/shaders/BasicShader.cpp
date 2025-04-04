#include <graphics/core/shaders/BasicShader.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
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
	BasicMaterialPtr mat = std::dynamic_pointer_cast<BasicMaterial>(material);
	if (!mat)
	{
		throw IllegalArgumentException("BasicShader may only accept BasicMaterials");
	}

	// Color
	glUniform4f(getUniformLocation("color"), mat->color.red(), mat->color.green(), mat->color.blue(), mat->color.alpha());

	// Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mat->texture->id());
	glUniform1i(getUniformLocation("tex"), GL_TEXTURE0);

	// Vertex color usage
	glUniform1i(getUniformLocation("useVertexColors"), mat->useVertexColors ? 1 : 0);
}