#include <graphics/core/shaders/PostProcessShader.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

PostProcessShader::PostProcessShader(): Shader("PostProcessShader", POST_PROCESS_VERTEX, POST_PROCESS_FRAGMENT)
{
}

PostProcessShaderPtr PostProcessShader::create()
{
	return std::shared_ptr<PostProcessShader>(new PostProcessShader());
}

void PostProcessShader::setMaterial(const Material* material)
{
	if (!material || material->materialType != MaterialType::POST_PROCESS)
	{
		throw IllegalArgumentException("PostProcessShader requires a PostProcessMaterial");
	}

	ProgramBinding binding(this);
	Shader::setMaterial(material);
	const PostProcessMaterial* ppMat = static_cast<const PostProcessMaterial*>(material);

	if (!ppMat->texture)
	{
		throw IllegalArgumentException("PostProcessMaterial requires an input texture");
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ppMat->texture->id());
	glUniform1i(getUniformLocation("uMaterial.texture"), 0);
	glUniform4f(getUniformLocation("uMaterial.color"), ppMat->color.red(), ppMat->color.green(), ppMat->color.blue(), ppMat->color.alpha());
	glUniform1f(getUniformLocation("uMaterial.exposure"), ppMat->exposure);
	glUniform1f(getUniformLocation("uMaterial.saturation"), ppMat->saturation);
	glUniform1f(getUniformLocation("uMaterial.contrast"), ppMat->contrast);
}
