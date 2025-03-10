#pragma once
#include <string>

class Shader;

/**
 * Constructs a new shader instance containing valid vertex shader source code
 * @param name The unique name to give to this shader
 */
Shader createExampleVertexShader(const std::string& name);

/**
 * Constructs a new shader instance containing valid fragment shader source code
 * @param name The unique name to give to this shader
 */
Shader createExampleFragmentShader(const std::string& name);