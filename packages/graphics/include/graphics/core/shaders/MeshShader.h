#pragma once
#include <graphics/core/shaders/pointers/MeshShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

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

/**
 * Shader used to handle meshes and their materials
 * @author Nathaniel Rex
 */
class MeshShader : public Shader
{
public:

	/**
	 * Constructs a new MeshShader instance. This should typically only be done once, by the shader manager.
	 * @return The new MeshShader instance
	 */
	static MeshShaderPtr create()
	{
		return std::shared_ptr<MeshShader>(new MeshShader());
	}

	void setItem(const RenderItem& item) override;

	void setCamera(Camera* camera) override;

	void setMaterial(const Material* material) override;

	void setLighting(const Lighting& lighting) override;

	void setEnvironment(const Environment& environment) override;

	/**
     * Updates uniforms for this shader using the given model matrix
     * @param matrix Matrix representing the transformation from local to world space
     */
	void setModelMatrix(const Matrix4& matrix);

	/**
     * Updates the uniforms for this shader using the given view matrix
     * @param matrix Matrix representing the transformation from world to view space
     */
    void setViewMatrix(const Matrix4& matrix);

	/**
     * Updates the uniforms for this shader using the given projection matrix
     * @param matrix Matrix representing the transformation from view to clipping space
     */
    void setProjectionMatrix(const Matrix4& matrix);

	/**
     * Updates the uniforms for this shader using the given normal matrix. This matrix is typically used to
	 * transform surface normals from local space to world space, without affecting scaling or translation.
     * @param matrix Matrix representing the transformation from local to world space for normal vectors
     */
    void setNormalMatrix(const Matrix3& matrix);

private:

	/**
	 * Constructor
	 */
	MeshShader(): Shader("MeshShader", MESH_VERTEX, MESH_FRAGMENT) {}
};
