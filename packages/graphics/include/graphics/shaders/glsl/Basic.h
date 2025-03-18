#pragma once

namespace Shaders {

	/**
	 * Source code for the vertex shader of the 'basic' program
	 */
	constexpr char* BASIC_VERTEX = R"(
		#version 330 core

		// Inputs
		layout (location = 0) in vec3 vert_Pos;
		layout (location = 1) in vec4 vert_Color;
		layout (location = 2) in vec2 vert_TexCoord;

		// Outputs
		out vec4 frag_Color;
		out vec2 frag_TexCoord;

		void main()
		{
			gl_Position = vec4(vert_Pos, 1.0);
			frag_Color = vert_Color;
			frag_TexCoord = vert_TexCoord;
		}
		)";

	/**
	 * Source code for the fragment shader of the 'basic' program
	 */
	constexpr char* BASIC_FRAGMENT = R"(
		#version 330 core

		// Inputs
		in vec4 frag_Color;
		in vec2 frag_TexCoord;

		// Uniforms
		uniform sampler2D uTexture;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			FragColor = texture(uTexture, frag_TexCoord) * frag_Color;
		} 
		)";
};