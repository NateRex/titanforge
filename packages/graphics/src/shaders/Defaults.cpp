#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderProgram.h>
#include <glad/glad.h>

// ----------------------------------------------------------------------------------------------
// Shaders
// ----------------------------------------------------------------------------------------------

Shader vertex(GL_VERTEX_SHADER, "tf_vertex", R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)");

Shader fragment(GL_FRAGMENT_SHADER, "tf_fragment", R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)");


// ----------------------------------------------------------------------------------------------
// Programs
// ----------------------------------------------------------------------------------------------

ShaderProgram defaultProgram("default", { "tf_vertex", "tf_fragment" });


void ShaderManager::linkDefaults()
{
	ShaderManager::mountShader(vertex);
	ShaderManager::mountShader(fragment);

	ShaderManager::linkProgram(defaultProgram);

	ShaderManager::unmountShaders();

	ShaderManager::useProgram("tf_default");
}