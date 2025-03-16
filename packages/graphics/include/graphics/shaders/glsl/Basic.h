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

		// Outputs
		out vec4 frag_Color;

		void main()
		{
			frag_Color = vert_Color;
			gl_Position = vec4(vert_Pos, 1.0);
		}
		)";

	/**
	 * Source code for the fragment shader of the 'basic' program
	 */
	constexpr char* BASIC_FRAGMENT = R"(
		#version 330 core

		// Inputs
		in vec4 frag_Color;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			FragColor = frag_Color;
		} 
		)";
};