#pragma once
#include <string>

/**
 * A shader program, managed by the shader manager
 * @author Nathaniel Rex
 */
class Shader {
public:

    friend class ShaderManager;

private:

    /**
	 * GLFW id. Will be zero until this program has been linked.
	 */
	unsigned int _id;

	/**
	 * Unique name of this shader program
	 */
	const std::string _name;

    /**
     * Constructor
     * @param name The unique name of this shader
     */
    Shader(const std::string& name);

    /**
     * Compiles shader code for use in linking.
     * @param type The shader type
     * @param source Shader source code
     * @return The ID of the compiled shader. Must be destroyed via glDeleteShader when no longer needed.
     */
    unsigned int compileSource(int type, const char* source);

    /**
     * Links this shader program for use by OpenGL, using the given source code.
     * @param vertexShader Vertex shader source code
     * @param fragmentShader Fragment shader source code
     * @throws InstantiationException On failure to compile or link the shader
     */
    void link(const char* vertexShader, const char* fragmentShader);

    /**
     * Destroys this shader program and releases all of its resources.
     */
    void destroy();
};