#pragma once
#include <graphics/core/shaders/Shader.h>

/**
 * Shader that generates and draws an oversized fullscreen triangle without geometry buffers.
 * @author Nathaniel Rex
 */
class FullScreenShader : public Shader
{
public:
    
    /**
     * Destructor
     */
    ~FullScreenShader();

    /**
     * Activates this shader and draws its fullscreen triangle into the configured destination.
     */
    void draw();

protected:
    
    /**
     * Constructs a fullscreen shader from a fragment shader consuming vec2 frag_TexCoord.
     * @param programName Program name. Used in error messages on failure to compile or link.
     * @param fragmentSource Fragment shader source code
     */
    FullScreenShader(const char* programName, const char* fragmentSource);

private:

    /**
     * Vertex array used to hold the 3 vertices that make up the full-screen triangle
     */
    unsigned int _vertexArray = 0;
};
