#pragma once
#include <string>

class Shader;

/**
 * Constructs a new shader instance containing valid vertex shader source code
 * @param name The unique name to give to this shader
 */
Shader createExampleShader(const std::string& name);