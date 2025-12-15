#pragma once
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>

class Matrix4;

/**
 * Parent class to all shared programs, managed by the shader manager
 * @author Nathaniel Rex
 */
class Shader {
public:

    /**
     * Destructor
     */
    ~Shader();

    /**
     * Updates uniforms for this shader using the given model matrix. This method assumes that this shader
     * is currently in-use.
     * @param matrix Matrix representing the transformation from local to world space
     */
    void setModelMatrix(const Matrix4& matrix);

    /**
     * Updates the uniforms for this shader using the given view matrix. This method assumes that this shader
     * is currently in-use.
     * @param matrix Matrix representing the transformation from world to view space
     */
    void setViewMatrix(const Matrix4& matrix);

    /**
     * Updates the uniforms for this shader using the given projection matrix. This method assumes that this
     * shader is currently in-use.
     * @param matrix Matrix representing the transformation from view to clipping space
     */
    void setProjectionMatrix(const Matrix4& matrix);

    /**
     * Updates the uniforms for this shader using the given ambient lighting. This method assumes that this shader
     * is currently in-use.
     * @param light Ambient lighting
     */
    void setAmbientLighting(const LightPtr light);

    /**
     * Updates the uniforms for this shader using the given light present in the scene. This method assumes that this
     * shader is currently in-use.
     * @param light Light entity
     */
    void setLight(const LightPtr light);

    /**
     * Updates uniforms for this shader using the given material. This method assumes that this shader is
     * currently in-use.
     * @param material Material
     */
    virtual void setMaterial(const MaterialPtr material) = 0;

    /**
     * Activates this shader as the current shader program used for rendering
     */
    void activate() const;

protected:

    /**
	 * GLFW id
	 */
	unsigned int _id;

    /**
     * Constructor
     * @param prgmName Program name. Used in error messages on failure to compile or link.
     * @param vertexShader Vertex shader source code
     * @param fragmentShader Fragment shader source code
     * @throws InstantiationException On failure to compile or link the shader
     */
    Shader(const char* prgmName, const char* vertexShader, const char* fragmentShader);

    /**
     * Compiles shader code for use in linking.
     * @param prgmName Program name. Used in error messages on failure to compile.
     * @param type The shader type
     * @param source Shader source code
     * @return The ID of the compiled shader. Must be destroyed via glDeleteShader when no longer needed.
     */
    unsigned int compileSource(const char* prgmName, int type, const char* source);

    /**
     * Helper method that obtains the location of a uniform variable in this shader program (assuming it's bound),
     * asserting that the variable exists in the process.
     * @param variableName The name of the variable
     * @return The variable location
     */
    int getUniformLocation(const char* variableName) const;
};