#include <graphics/core/shaders/MeshShader.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/textures/Texture.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/cameras/Camera.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/renderer/RenderState.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>
#include <cmath>

MeshShader::MeshShader() : Shader("MeshShader", MESH_VERTEX, MESH_FRAGMENT)
{

}

MeshShaderPtr MeshShader::create()
{
	return std::shared_ptr<MeshShader>(new MeshShader());
}

void MeshShader::setItem(const RenderItem& item)
{
	Shader::setItem(item);
	setModelMatrix(item.modelTransform);
	setNormalMatrix(item.normalTransform);
}

void MeshShader::setCamera(Camera* camera)
{
	Shader::setCamera(camera);
	ProgramBinding binding(this);

	Vector3 cameraPos = camera->getPosition();
	glUniform3f(getUniformLocation("uCameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	setViewMatrix(camera->getViewMatrix());
	setProjectionMatrix(camera->getProjectionMatrix());
}

void MeshShader::setMaterial(const Material* material)
{
	if (!material || material->materialType != MaterialType::MESH)
	{
		throw IllegalArgumentException("MeshShader requires a MeshMaterial");
	}

	Shader::setMaterial(material);
	ProgramBinding binding(this);
	const MeshMaterial* meshMat = static_cast<const MeshMaterial*>(material);

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
	glUniform1f(getUniformLocation("uMaterial.reflection"), clamp(meshMat->reflection, 0.f, 1.f));
	glUniform1f(getUniformLocation("uMaterial.refraction"), clamp(meshMat->refraction, 0.f, 1.f));
	glUniform1f(getUniformLocation("uMaterial.refractiveIndex"), std::max(meshMat->refractiveIndex, 1.f));

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

void MeshShader::setEnvironment(const Environment& environment)
{
	ProgramBinding binding(this);
	if (!environment.texture)
	{
		glUniform1i(getUniformLocation("uHasEnvironment"), 0);
		return;
	}

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environment.texture->id());
	glUniform1i(getUniformLocation("uEnvironment"), 3);
	glUniform1i(getUniformLocation("uHasEnvironment"), 1);
	glUniform3f(getUniformLocation("uEnvironmentColor"), environment.color.red(), environment.color.green(), environment.color.blue());
	glUniform1f(getUniformLocation("uEnvironmentIntensity"), std::max(environment.intensity, 0.f));
	glUniform1f(getUniformLocation("uEnvironmentRotation"), environment.rotation);
	glUniform1f(getUniformLocation("uEnvironmentLod"), std::max(environment.lod, 0.f));
}

void MeshShader::setLighting(const Lighting& lighting)
{
	ProgramBinding binding(this);
	Shader::setLighting(lighting);

	constexpr size_t MAX_LIGHTS = 16;
	float ambientRed = 0.f;
	float ambientGreen = 0.f;
	float ambientBlue = 0.f;
	size_t lightCount = 0;

	for (const RenderLight& renderLight : lighting.lights)
	{
		const Light* light = renderLight.light;
		if (!light)
		{
			continue;
		}

		const Color color = light->color;
		const float intensity = std::max(light->intensity, 0.f);

		// In the case of ambient lighting, aggregate values across all ambient lights
		if (light->lightType == LightType::AMBIENT)
		{
			ambientRed += color.red() * intensity;
			ambientGreen += color.green() * intensity;
			ambientBlue += color.blue() * intensity;
			continue;
		}

		// Stop setting uniforms if we've reached the maximum number of lights supported
		if (lightCount >= MAX_LIGHTS)
		{
			continue;
		}

		// Compute the homogeneous light vector, which works differently depending on the type of light
		Vector3 vector;
		float w = 1.f;
		if (light->lightType == LightType::DIRECTIONAL)
		{
			// Forward is the direction the rays travel; shading needs the
			// opposite direction, from the surface toward the light.
			vector = renderLight.direction.scale(-1.f);
			w = 0.f;
		}
		else
		{
			vector = renderLight.position;
		}

		// Compute distance attenuation for local lights
		float constantAttenuation = 1.f;
		float linearAttenuation = 0.f;
		float quadraticAttenuation = 0.f;
		float range = light->range;
		if (light->attenuation && range > 0.f)
		{
			// Scale the familiar lamp-like (1, 0.09, 0.032) curve so
			// `range` remains the only distance users need to reason about.
			linearAttenuation = 4.5f / range;
			quadraticAttenuation = 80.f / (range * range);
		}

		// Encode cone falloff homogeneously. The neutral values make smoothstep
		// evaluate to one for every possible dot product on non-spot lights.
		Vector3 direction = Vector3::ZERO;
		float outerCutoff = -2.f;
		float innerCutoff = -1.f;
		if (light->lightType == LightType::SPOTLIGHT)
		{
			const SpotLight* spotLight = static_cast<const SpotLight*>(light);
			const float innerAngle = clamp(spotLight->innerAngle, 0.f, 180.f);
			const float outerAngle = clamp(spotLight->outerAngle, innerAngle, 180.f);
			direction = renderLight.direction;
			outerCutoff = std::cos(deg2Rad(outerAngle));
			innerCutoff = std::cos(deg2Rad(innerAngle));
			// GLSL smoothstep requires distinct edges.
			if (innerCutoff - outerCutoff < 0.00001f)
			{
				outerCutoff = innerCutoff - 0.00001f;
			}
		}

		// Assign uniforms
		const std::string prefix = "uLights[" + std::to_string(lightCount) + "].";
		glUniform4f(getUniformLocation((prefix + "vector").c_str()), vector.x, vector.y, vector.z, w);
		glUniform3f(getUniformLocation((prefix + "color").c_str()), color.red(), color.green(), color.blue());
		glUniform1f(getUniformLocation((prefix + "intensity").c_str()), intensity);
		glUniform3f(
			getUniformLocation((prefix + "attenuation").c_str()),
			constantAttenuation,
			linearAttenuation,
			quadraticAttenuation);
		glUniform3f(getUniformLocation((prefix + "direction").c_str()), direction.x, direction.y, direction.z);
		glUniform2f(getUniformLocation((prefix + "cone").c_str()), outerCutoff, innerCutoff);
		++lightCount;
	}

	glUniform3f(getUniformLocation("uAmbient"), ambientRed, ambientGreen, ambientBlue);
	glUniform1i(getUniformLocation("uLightCount"), lightCount);
}

void MeshShader::setModelMatrix(const Matrix4& matrix)
{
	ProgramBinding binding(this);
	glUniformMatrix4fv(getUniformLocation("uTransforms.model"), 1, GL_TRUE, matrix.getValues());
}

void MeshShader::setViewMatrix(const Matrix4& matrix)
{
	ProgramBinding binding(this);
	glUniformMatrix4fv(getUniformLocation("uTransforms.view"), 1, GL_TRUE, matrix.getValues());
}

void MeshShader::setProjectionMatrix(const Matrix4& matrix)
{
	ProgramBinding binding(this);
	glUniformMatrix4fv(getUniformLocation("uTransforms.proj"), 1, GL_TRUE, matrix.getValues());
}

void MeshShader::setNormalMatrix(const Matrix3& matrix)
{
	ProgramBinding binding(this);
	glUniformMatrix3fv(getUniformLocation("uTransforms.normal"), 1, GL_TRUE, matrix.getValues());
}
