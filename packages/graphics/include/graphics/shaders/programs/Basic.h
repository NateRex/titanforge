#pragma once
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderProgram.h>
#include <glad/glad.h>

/**
 * Vertex shader
 */
Shader basic_shader_vertex(GL_VERTEX_SHADER, "tf_basic_vertex", R"(
#version 330 core

// Inputs
layout (location = 0) in vec3 aPos;

// Outputs
out vec4 gl_Position;
out vec4 color;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	color = vec4(0.5, 0.0, 0.0, 1.0);
}
)");


/**
 * Fragment shader
 */
Shader basic_shader_fragment(GL_FRAGMENT_SHADER, "tf_basic_fragment", R"(
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

/**
 * Program
 */
ShaderProgram basic_shader_program("tf_basic", { "tf_basic_vertex", "tf_basic_fragment" });