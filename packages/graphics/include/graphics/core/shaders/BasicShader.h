#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/core/shaders/pointers/BasicShaderPtr.h>

/**
 * Source code for the vertex shader used to handle 'basic' materials
 */
constexpr const char* BASIC_VERTEX = R"(
		#version 330 core

		// Inputs
		layout (location = 0) in vec3 vert_Pos;
		layout (location = 1) in vec4 vert_Color;
		layout (location = 2) in vec2 vert_TexCoord;

		// Uniforms
		uniform vec4 uColor;
		uniform int uUseVertexColors;
		uniform mat4 uModel;
		uniform mat4 uView;
		uniform mat4 uProj;

		// Outputs
		out vec4 frag_Color;
		out vec2 frag_TexCoord;

		void main()
		{
			gl_Position = uProj * uView * uModel * vec4(vert_Pos, 1.0f);
			frag_Color = uUseVertexColors == 1 ? vert_Color : uColor;
			frag_TexCoord = vert_TexCoord;
		}
)";

/**
 * Source code for the fragment shader used to handle 'basic' materials
 */
constexpr const char* BASIC_FRAGMENT = R"(
		#version 330 core

		// Inputs
		in vec4 frag_Color;
		in vec2 frag_TexCoord;

		// Uniforms
		uniform int uHasTexture;
		uniform sampler2D uTexture;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			vec4 color = frag_Color;
			if (uHasTexture == 1) {
				color *= texture(uTexture, frag_TexCoord);
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

	/**
	 * Updates uniforms for this shader using the given material. This method assumes that this shader is
	 * currently in-use.
	 * @param material Material
	 */
	void setMaterial(const MaterialPtr material) override;

private:

	/**
	 * Constructor
	 */
	BasicShader();
};