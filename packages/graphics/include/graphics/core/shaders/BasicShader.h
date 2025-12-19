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

		struct Material {
			vec4 color;
			int hasVertexColor;
			float reflectivity;
			float shine;
			int hasTexture;
			sampler2D texture;
		};

		// Inputs
		layout (location = 0) in vec3 vert_Pos;
		layout (location = 1) in vec3 vert_Normal;
		layout (location = 2) in vec4 vert_Color;
		layout (location = 3) in vec2 vert_TexCoord;

		// Uniforms
		uniform Material uMaterial;
		uniform Transforms uTransforms;

		// Outputs
		out vec4 frag_Color;
		out vec3 frag_Pos;
		out vec3 frag_Normal;
		out vec2 frag_TexCoord;

		void main()
		{
			frag_Color = uMaterial.hasVertexColor == 1 ? vert_Color : uMaterial.color;
			frag_Pos = vec3(uTransforms.model * vec4(vert_Pos, 1.0f));
			frag_Normal = uTransforms.normal * vert_Normal;
			frag_TexCoord = vert_TexCoord;
			gl_Position = uTransforms.proj * uTransforms.view * uTransforms.model * vec4(vert_Pos, 1.0f);
		}
)";

/**
 * Source code for the fragment shader used to handle 'basic' materials
 */
constexpr const char* BASIC_FRAGMENT = R"(
		#version 330 core

		struct Light {
			vec3 position;
			vec3 color;
			float intensity;
		};

		struct Material {
			vec4 color;
			int hasVertexColor;
			float reflectivity;
			float shine;
			int hasTexture;
			sampler2D texture;
		};

		// Inputs
		in vec4 frag_Color;
		in vec3 frag_Pos;
		in vec3 frag_Normal;
		in vec2 frag_TexCoord;

		// Uniforms
		uniform vec3 uCameraPos;
		uniform Light uAmbient;
		uniform Light uLight;
		uniform Material uMaterial;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			vec4 ambient = vec4(uAmbient.color * uAmbient.intensity, 1.0);
			
			vec4 diffuse = vec4(uLight.color * uLight.intensity, 1.0);
			vec3 norm = normalize(frag_Normal);
			vec3 lightDir = normalize(uLight.position - frag_Pos);
			float diff = max(dot(norm, lightDir), 0.0);
			diffuse = diff * diffuse;

			vec3 viewDir = normalize(uCameraPos - frag_Pos);
			vec3 reflectDir = reflect(-lightDir, norm);
			float specExp = exp2(round(mix(0.0, 8.0, uMaterial.shine)));
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);
			vec4 specular = vec4(uMaterial.reflectivity * spec * uLight.color, 1.0);  

			vec4 color = (ambient + diffuse + specular) * frag_Color;
			if (uMaterial.hasTexture == 1)
			{
				color *= texture(uMaterial.texture, frag_TexCoord);
			}
			FragColor = color;
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