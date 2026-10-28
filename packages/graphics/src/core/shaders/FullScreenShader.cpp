#include <graphics/core/shaders/FullScreenShader.h>
#include <glad/glad.h>

constexpr const char* FULL_SCREEN_VERTEX = R"(
    #version 330 core

    out vec2 frag_TexCoord;

    void main()
    {
        vec2 position = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));
        frag_TexCoord = position;
        gl_Position = vec4(position * 2.0 - 1.0, 0.0, 1.0);
    }
)";

FullScreenShader::FullScreenShader(const char* programName, const char* fragmentSource)
    : Shader(programName, FULL_SCREEN_VERTEX, fragmentSource) {}

FullScreenShader::~FullScreenShader()
{
    if (_vertexArray != 0)
    {
        glDeleteVertexArrays(1, &_vertexArray);
    }
}

void FullScreenShader::draw()
{
    activate();

    if (_vertexArray == 0)
    {
        glGenVertexArrays(1, &_vertexArray);
    }
    glBindVertexArray(_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
