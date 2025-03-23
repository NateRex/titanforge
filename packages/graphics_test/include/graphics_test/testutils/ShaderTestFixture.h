#pragma once

/**
 * Shader definitions
 */
namespace Shaders {

	/**
	 * Source code for the vertex shader of the 'test' program
	 */
	constexpr const char* TEST_VERTEX = R"(
		#version 330 core

		// Inputs
		layout (location = 0) in vec3 vert_Pos;
		layout (location = 1) in vec4 vert_Color;
		layout (location = 2) in vec2 vert_TexCoord;

		// Uniforms
		uniform mat4 matrix1;

		// Outputs
		out vec4 frag_Color;
		out vec2 frag_TexCoord;

		void main()
		{
			gl_Position = matrix1 * vec4(vert_Pos, 1.0f);
			frag_Color = vert_Color;
			frag_TexCoord = vert_TexCoord;
		}
		)";

	/**
	 * Source code for the fragment shader of the 'test' program
	 */
	constexpr const char* TEST_FRAGMENT = R"(
		#version 330 core

		// Inputs
		in vec4 frag_Color;
		in vec2 frag_TexCoord;

		// Uniforms
		uniform sampler2D texture1;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			FragColor = texture(texture1, frag_TexCoord) * frag_Color;
		} 
		)";
};

/**
 * A test fixture that creates a temporary shader named "test" inside of the shader manager.
 * This shader is configured with a uniform variable of every type, and is intended to be used
 * in testing without having to worry about code changes to main files.
 * @author Nathaniel Rex
 */
class ShaderTestFixture
{
public:

    /**
     * Code to run before all tests
     */
	ShaderTestFixture();

    /**
     * Code to run after all tests
     */
    ~ShaderTestFixture();
};