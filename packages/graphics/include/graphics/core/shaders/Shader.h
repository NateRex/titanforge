#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>

class Matrix3;
class Matrix4;
struct RenderState;
struct RenderItem;
struct Lighting;

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
     * Updates this shader's uniforms given a render state
     * @param state Render state generated via scene traversal
     */
    virtual void setState(const RenderState& state);

    /**
     * Updates this shader's uniforms given a specific item being rendered. This automatically applies the item's
     * material as well.
     * @param item Item being rendered
     */
    virtual void setItem(const RenderItem& item);

    /**
     * Updates uniforms for this shader using the given camera. This method assumes that this shader is currently in-use.
     * @param camera Camera
     */
    virtual void setCamera(const CameraPtr camera) {}

    /**
     * Updates uniforms for this shader using the given material. This method assumes that this shader is
     * currently in-use.
     * @param material Material
     */
    virtual void setMaterial(const MaterialPtr material) {}

    /**
     * Updates the uniforms for this shader using the given lighting information. This method assumes that this shader
     * is currently in-use.
     * @param lighting Lights affecting the render pass
     */
    virtual void setLighting(const Lighting& lighting) {}

    /**
     * Activates this shader as the current shader program used for rendering
     */
    void activate() const;

protected:

    /**
	 * OpenGL program object name
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
