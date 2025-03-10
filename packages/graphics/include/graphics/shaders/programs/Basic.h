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
layout (location = 0) in vec3 vert_Pos;
layout (location = 1) in vec4 vert_Color;

// Outputs
out vec4 frag_Color;

void main()
{
	frag_Color = vert_Color;
	gl_Position = vec4(vert_Pos, 1.0);
}
)");


/**
 * Fragment shader
 */
Shader basic_shader_fragment(GL_FRAGMENT_SHADER, "tf_basic_fragment", R"(
#version 330 core

// Inputs
in vec4 frag_Color;

// Outputs
out vec4 FragColor;

void main()
{
    FragColor = frag_Color;
} 
)");

/**
 * Program
 */
ShaderProgram basic_shader_program("tf_basic", { "tf_basic_vertex", "tf_basic_fragment" });