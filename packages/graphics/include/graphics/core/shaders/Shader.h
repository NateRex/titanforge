#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>

class Matrix3;
class Matrix4;
struct RenderState;
struct RenderItem;
struct Lighting;
struct Environment;

/**
 * Parent class to all shader programs, which are typically managed by the shader manager
 * @author Nathaniel Rex
 */
class Shader {
public:

    /**
     * Destructor
     */
    ~Shader();

    /**
     * @return The OpenGL ID of this shader
     */
    unsigned int id() const { return _id; }

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
     * Updates uniforms for this shader using the given camera.
     * @param camera Camera
     */
    virtual void setCamera(const CameraPtr camera) {}

    /**
     * Updates uniforms for this shader using the given material.
     * @param material Material
     */
    virtual void setMaterial(const MaterialPtr material) {}

    /**
     * Updates the uniforms for this shader using the given lighting information.
     * @param lighting Lights affecting the render pass
     */
    virtual void setLighting(const Lighting& lighting) {}

    /**
     * Updates uniforms for this shader using the given environment map
     * @param environment Environment settings
     */
    virtual void setEnvironment(const Environment& environment) {}

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
     * Constructor
     * @param prgmName Program name. Used in error messages on failure to compile or link.
     * @param vertexShader Vertex shader source code
     * @param geometryShader Geometry shader source code. Can be null, in which case no custom geometry shader will be applied.
     * @param fragmentShader Fragment shader source code
     */
    Shader(const char* prgmName, const char* vertexShader, const char* geometryShader, const char* fragmentShader);

    /**
     * Compiles shader code for use in linking.
     * @param prgmName Program name. Used in error messages on failure to compile.
     * @param type The shader type
     * @param source Shader source code
     * @return The ID of the compiled shader. Must be destroyed via glDeleteShader when no longer needed.
     */
    unsigned int compileSource(const char* prgmName, int type, const char* source);

    /**
     * Helper method that obtains the location of a uniform variable in this shader program, asserting that the
     * variable exists in the process.
     * @param variableName The name of the variable
     * @return The variable location
     */
    int getUniformLocation(const char* variableName) const;
};

/**
 * A temporary shader program binding, used to activate a shader temporarily. On destruction, the previously active
 * shader program is restored.
 * @author Nathaniel Rex
 */
class ProgramBinding
{
public:

    /**
     * Constructor
     * @param shader The shader program to temporarily bind
     */
    ProgramBinding(const Shader* shader);

    /**
     * Destructor
     */
    ~ProgramBinding();

private:

    /**
     * Boolean flag that, when true, indicates that the shader program temporarily bound differs from the program
     * that was previously active.
     */
    bool _changedProgram;

    /**
     * The shader program that was active previous to the creation of this binding
     */
    unsigned int _previousProgram;
};
