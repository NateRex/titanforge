#include <graphics/core/shaders/MeshShader.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/textures/Texture.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/cameras/Camera.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/renderer/DrawState.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>
#include <cmath>

/**
 * Source code for the vertex shader used to handle mesh materials
 */
constexpr const char* MESH_VERTEX = R"(
	#version 330 core

	struct Transforms {
		mat4 model;
		mat4 view;
		mat4 proj;
		mat3 normal;
	};

	layout (location = 0) in vec3 vert_Pos;
	layout (location = 1) in vec3 vert_Normal;
	layout (location = 2) in vec4 vert_VertexColor;
	layout (location = 3) in vec2 vert_TexCoord;

	uniform Transforms uTransforms;

	out vec3 frag_Pos;
	out vec3 frag_Normal;
	out vec4 frag_VertexColor;
	out vec2 frag_TexCoord;

	void main()
	{
		// Mark vertex as a position and translate from local coordinates to world space
		vec4 worldPos = uTransforms.model * vec4(vert_Pos, 1.0);
		frag_Pos = worldPos.xyz;

		// Transform surface directions into world space
		frag_Normal = uTransforms.normal * vert_Normal;

		// Passthrough the vertex's RGBA color
		frag_VertexColor = vert_VertexColor;

		// Passthrough texture coordinates
		frag_TexCoord = vert_TexCoord;

		// Transform from world position to camera's coordinate system, and then project into clip space
		gl_Position = uTransforms.proj * uTransforms.view * worldPos;
	}
)";

/**
 * Source code for the fragment shader used to handle mesh materials
 */
constexpr const char* MESH_FRAGMENT = R"(
	#version 330 core
	#define MAX_LIGHTS 16

	struct Light {
		// Homogeneous light vector: point and spotlight position with w=1, or the direction
		// toward a directional light with w=0.
		vec4 vector;
		vec3 color;
		float intensity;
		// constant, linear, and quadratic distance attenuation coefficients
		vec3 attenuation;
		// Direction rays travel, followed by cosine of outer/inner cone angles.
		// Non-spot lights receive neutral cone values.
		vec3 direction;
		vec2 cone;
	};

	struct Material {
		vec4 color;
		int alphaMode;
		float alphaCutoff;
		float reflectivity;
		float shine;
		int hasVertexColor;
		int hasTexture;
		sampler2D texture;
		int hasDiffuseMap;
		sampler2D diffuseMap;
		int hasSpecularMap;
		sampler2D specularMap;
		float reflection;
		float refraction;
		float refractiveIndex;
	};

	in vec3 frag_Pos;
	in vec3 frag_Normal;
	in vec4 frag_VertexColor;
	in vec2 frag_TexCoord;

	uniform vec3 uCameraPos;
	uniform vec3 uAmbient;
	uniform int uLightCount;
	uniform Light uLights[MAX_LIGHTS];
	uniform Material uMaterial;
	uniform int uHasEnvironment;
	uniform samplerCube uEnvironment;
	uniform vec3 uEnvironmentColor;
	uniform float uEnvironmentIntensity;
	uniform float uEnvironmentRotation;
	uniform float uEnvironmentLod;

	out vec4 FragColor;

	void main()
	{
		// Choose surface's starting RGBA color
		vec4 albedo = uMaterial.hasVertexColor == 1
			? frag_VertexColor
			: uMaterial.color;

		// Sample color from the texture if one is available
		if (uMaterial.hasTexture != 0)
		{
			albedo *= texture(uMaterial.texture, frag_TexCoord);
		}

		// Apply alpha cutoff in MASK mode, discarding the fragment if it doesn't meet the cutoff
		if (uMaterial.alphaMode == 2 && albedo.a < uMaterial.alphaCutoff)
		{
			discard;
		}

		vec3 normal = normalize(frag_Normal);
		vec3 viewDir = normalize(uCameraPos - frag_Pos);
		vec3 diffuse = vec3(0.0);
		vec3 specular = vec3(0.0);
		vec3 diffuseReflectance = uMaterial.hasDiffuseMap != 0
			? texture(uMaterial.diffuseMap, frag_TexCoord).rgb
			: vec3(1.0);
		vec3 specularReflectance = uMaterial.hasSpecularMap != 0
			? texture(uMaterial.specularMap, frag_TexCoord).rgb
			: vec3(1.0);

		// Convert shine setting from [0, 1] into an exponent from [2, 256]
		float shininess = mix(2.0, 256.0, uMaterial.shine);

		for (int i = 0; i < uLightCount; ++i)
		{
			Light light = uLights[i];

			// For a point/spot light, w=1 so this is light position minus pixel position
			// For a directional light, w=0 so we end up with just the direction of the light
			vec3 lightDir = normalize(light.vector.xyz - frag_Pos * light.vector.w);
			float distance = length(light.vector.xyz - frag_Pos);

			// Light fades as 1 divided by (constant + linear*d + quadratic*d*d)
			float attenuation = 1.0 / (
				light.attenuation.x
				+ light.attenuation.y * distance
				+ light.attenuation.z * distance * distance);

			// In the case of a cone light, intensity will be 0 outside the outer cone, 1 inside the inner cone
			// and soft blend between the two
			float coneIntensity = smoothstep(
				light.cone.x,
				light.cone.y,
				dot(-lightDir, light.direction));
				
			// Light hitting fragment = color, scaled by brightness, then distance fading, and finally spotlight-cone fading
			vec3 radiance = light.color
				* light.intensity
				* attenuation
				* coneIntensity;

			// Add matte lighting, reduced by the surface/light angle
			float nDotL = max(dot(normal, lightDir), 0.0);
			diffuse += radiance * nDotL;

			// Blinn-Phong highlight. Shine maps [0, 1] to a useful exponent range.
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float specularFactor = nDotL > 0.0
				? pow(max(dot(normal, halfwayDir), 0.0), shininess)
				: 0.0;
			specular += radiance
				* uMaterial.reflectivity
				* specularReflectance
				* specularFactor;
		}

		vec3 litColor = albedo.rgb * diffuseReflectance * (uAmbient + diffuse) + specular;

		// Apply environment reflection / refraction
		if (uHasEnvironment != 0)
		{
			vec3 incident = normalize(frag_Pos - uCameraPos);
			vec3 reflectedDirection = reflect(incident, normal);
			vec3 refractedDirection = refract(incident, normal, 1.0 / max(uMaterial.refractiveIndex, 1.0));
			float c = cos(uEnvironmentRotation);
			float s = sin(uEnvironmentRotation);
			reflectedDirection.xz = mat2(c, -s, s, c) * reflectedDirection.xz;
			refractedDirection.xz = mat2(c, -s, s, c) * refractedDirection.xz;
			vec3 reflected = textureLod(uEnvironment, reflectedDirection, uEnvironmentLod).rgb;
			vec3 refracted = textureLod(uEnvironment, refractedDirection, uEnvironmentLod).rgb;
			reflected *= uEnvironmentColor * uEnvironmentIntensity;
			refracted *= uEnvironmentColor * uEnvironmentIntensity;
			litColor = mix(litColor, reflected, uMaterial.reflection);
			litColor = mix(litColor, refracted, uMaterial.refraction);
		}
		FragColor = vec4(litColor, albedo.a);
	}
)";

MeshShader::MeshShader(): Shader("MeshShader", MESH_VERTEX, MESH_FRAGMENT)
{

}

MeshShader::MeshShader(const char* name, const char* vertex, const char* geometry, const char* fragment)
	: Shader(
		name,
		vertex ? vertex : MESH_VERTEX,
		geometry ? geometry : nullptr,
		fragment ? fragment : MESH_FRAGMENT)
{

}

void MeshShader::setItem(const DrawItem& item)
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

void MeshShader::setLighting(const std::vector<LightInstance>& lights)
{
	ProgramBinding binding(this);
	Shader::setLighting(lights);

	constexpr size_t MAX_LIGHTS = 16;
	float ambientRed = 0.f;
	float ambientGreen = 0.f;
	float ambientBlue = 0.f;
	size_t lightCount = 0;

	for (const LightInstance& renderLight : lights)
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
