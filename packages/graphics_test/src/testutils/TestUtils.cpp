#include <graphics_test/TestUtils.h>
#include <graphics/shaders/Shader.h>
#include <glad/glad.h>

Shader createExampleShader(const std::string& name)
{
	return Shader(GL_VERTEX_SHADER, name, R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)");
}