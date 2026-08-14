#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/core/shaders/pointers/BasicShaderPtr.h>

/**
 * Source code for the vertex shader used to handle 'basic' materials
 */
constexpr const char* BASIC_VERTEX = R"(
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
		vec4 worldPos = uTransforms.model * vec4(vert_Pos, 1.0);

		frag_Pos = worldPos.xyz;
		frag_Normal = uTransforms.normal * vert_Normal;
		frag_VertexColor = vert_VertexColor;
		frag_TexCoord = vert_TexCoord;

		gl_Position = uTransforms.proj * uTransforms.view * worldPos;
	}
)";

/**
 * Source code for the fragment shader used to handle 'basic' materials
 */
constexpr const char* BASIC_FRAGMENT = R"(
	#version 330 core
	#define MAX_LIGHTS 16

	struct Light {
		// Homogeneous light vector: point position with w=1, or the direction
		// toward a directional light with w=0.
		vec4 vector;
		vec3 color;
		float intensity;
		// constant, linear, and quadratic distance attenuation coefficients
		vec3 attenuation;
	};

	struct Material {
		vec4 color;
		float reflectivity;
		float shine;
		int hasVertexColor;
		int hasTexture;
		sampler2D texture;
		int hasDiffuseMap;
		sampler2D diffuseMap;
		int hasSpecularMap;
		sampler2D specularMap;
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

	out vec4 FragColor;

	void main()
	{
		vec4 albedo = uMaterial.hasVertexColor == 1
			? frag_VertexColor
			: uMaterial.color;

		if (uMaterial.hasTexture != 0)
		{
			albedo *= texture(uMaterial.texture, frag_TexCoord);
		}

		vec3 normal = normalize(frag_Normal);
		vec3 viewDir = normalize(uCameraPos - frag_Pos);
		vec3 diffuse = vec3(0.0);
		vec3 specular = vec3(0.0);
		vec3 diffuseReflectance = vec3(1.0);
		if (uMaterial.hasDiffuseMap != 0)
		{
			diffuseReflectance = texture(uMaterial.diffuseMap, frag_TexCoord).rgb;
		}

		float shininess = mix(2.0, 256.0, uMaterial.shine);
		vec3 specularReflectance = uMaterial.hasSpecularMap != 0
			? texture(uMaterial.specularMap, frag_TexCoord).rgb
			: vec3(1.0);

		for (int i = 0; i < uLightCount; ++i)
		{
			Light light = uLights[i];
			vec3 lightDir = normalize(light.vector.xyz - frag_Pos * light.vector.w);
			float nDotL = max(dot(normal, lightDir), 0.0);
			float distance = length(light.vector.xyz - frag_Pos);
			float attenuation = 1.0 / (
				light.attenuation.x
				+ light.attenuation.y * distance
				+ light.attenuation.z * distance * distance);
			vec3 radiance = light.color * light.intensity * attenuation;
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

		// Albedo colors ambient and diffuse light, but not the specular highlight.
		vec3 litColor = albedo.rgb * diffuseReflectance * (uAmbient + diffuse) + specular;
		FragColor = vec4(litColor, albedo.a);
	}
)";

/**
 * Shader used to handle basic materials
 * @author Nathaniel Rex
 */
class BasicShader : public Shader
{
public:

	/**
	 * Constructs a new BasicShader instance. This should typically only be done once, by the shader manager.
	 * @return The new BasicShader instance
	 */
	static BasicShaderPtr create();

protected:

	/**
	 * @override
	 * @param material Material
	 */
	void setMaterial(const MaterialPtr material) override;

private:

	/**
	 * Constructor
	 */
	BasicShader();
};
