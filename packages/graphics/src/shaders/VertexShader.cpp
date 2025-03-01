#include <graphics/shaders/VertexShader.h>
#include <glad/glad.h>

unsigned int type = GL_VERTEX_SHADER;
const char* name = "vertex";
const char* src = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

VertexShader::VertexShader(): IShader(type, name, src)
{

}