#include <graphics_test/TestUtils.h>
#include <graphics/shaders/Shader.h>
#include <glad/glad.h>

Shader createExampleVertexShader(const std::string& name)
{
	return Shader(GL_VERTEX_SHADER, name, R"(
		#version 330 core
	
		// Inputs
		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)");
}

Shader createExampleFragmentShader(const std::string& name)
{
	return Shader(GL_FRAGMENT_SHADER, name, R"(
		#version 330 core
	
		// Uniforms
		uniform vec4 color;

		// Outputs
		out vec4 FragColor;

		void main()
		{
			FragColor = color;
		}
		)");
}