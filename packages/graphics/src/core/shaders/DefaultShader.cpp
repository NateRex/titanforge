#include <graphics/core/shaders/DefaultShader.h>
#include <graphics/materials/MeshMaterial.h>
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
	if (!material || material->materialType != MaterialType::MESH)
	{
		throw IllegalArgumentException("MeshShader requires a MeshMaterial");
	}

	const MeshMaterialPtr meshMat = std::static_pointer_cast<MeshMaterial>(material);

	// Color
	int loc = getUniformLocation("uMaterial.color");
	Color color = material->color;
	glUniform4f(loc, color.red(), color.green(), color.blue(), color.alpha());

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

	// Alpha settings
	glUniform1i(getUniformLocation("uMaterial.alphaMode"), static_cast<int>(meshMat->getEffectiveAlphaMode()));
	glUniform1f(getUniformLocation("uMaterial.alphaCutoff"), meshMat->alphaCutoff);

	// Reflectivity and shine
	glUniform1f(getUniformLocation("uMaterial.reflectivity"), clamp(meshMat->reflectivity, 0.f, 1.f));
	glUniform1f(getUniformLocation("uMaterial.shine"), clamp(meshMat->shine, 0.f, 1.f));

	// Diffuse map
	if (meshMat->diffuseMap)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, meshMat->diffuseMap->id());
		glUniform1i(getUniformLocation("uMaterial.diffuseMap"), 1);
		glUniform1i(getUniformLocation("uMaterial.hasDiffuseMap"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasDiffuseMap"), 0);
	}

	// Specular map
	if (meshMat->specularMap)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, meshMat->specularMap->id());
		glUniform1i(getUniformLocation("uMaterial.specularMap"), 2);
		glUniform1i(getUniformLocation("uMaterial.hasSpecularMap"), 1);
	}
	else
	{
		glUniform1i(getUniformLocation("uMaterial.hasSpecularMap"), 0);
	}

	// Vertex color usage
	glUniform1i(getUniformLocation("uMaterial.hasVertexColor"), meshMat->useVertexColors ? 1 : 0);
}
