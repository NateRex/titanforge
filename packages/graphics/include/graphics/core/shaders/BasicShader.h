#pragma once
#include <graphics/core/shaders/Shader.h>
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
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 proj;

		// Outputs
		out vec4 frag_Color;
		out vec2 frag_TexCoord;

		void main()
		{
			gl_Position = proj * view * model * vec4(vert_Pos, 1.0f);
			frag_Color = vert_Color;
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
		uniform sampler2D tex;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			FragColor = texture(tex, frag_TexCoord);
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