#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <glad/glad.h>

Shader vertex(GL_VERTEX_SHADER, "vertex", R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)");

Shader fragment(GL_FRAGMENT_SHADER, "fragment", R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)");


void ShaderManager::loadDefaults()
{
	ShaderManager::loadShader(vertex);
	ShaderManager::loadShader(fragment);
}