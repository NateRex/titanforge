#include <graphics/core/shaders/WireframeShader.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/WireframeMaterial.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Source code for the vertex shader used to handle wireframe materials
 */
constexpr const char* WIREFRAME_VERTEX = R"(
    #version 330 core

    struct Transforms {
		mat4 model;
		mat4 view;
		mat4 proj;
	};

    layout (location = 0) in vec3 vert_Pos;

    uniform Transforms uTransforms;

    void main()
    {
        gl_Position = uTransforms.proj * uTransforms.view * uTransforms.model * vec4(vert_Pos, 1.0);
    }
)";

/**
 * Source code for the geometry shader used to expand lines to screen-facing quads
 */
constexpr const char* WIREFRAME_GEOMETRY = R"(
    #version 330 core

    layout (triangles) in;
    layout (line_strip, max_vertices = 6) out;

    void emitLine(int start, int end)
    {
        gl_Position = gl_in[start].gl_Position;
        EmitVertex();
        gl_Position = gl_in[end].gl_Position;
        EmitVertex();
        EndPrimitive();
    }

    void main()
    {
        emitLine(0, 1);
        emitLine(1, 2);
        emitLine(2, 0);
    }
)";

/**
 * Source code for the fragment shader used to handle line materials
 */
constexpr const char* WIREFRAME_FRAGMENT = R"(
    #version 330 core

    uniform vec4 uColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = uColor;
    }
)";

WireframeShader::WireframeShader() : Shader("WireframeShader", WIREFRAME_VERTEX, WIREFRAME_GEOMETRY, WIREFRAME_FRAGMENT)
{
    
}

void WireframeShader::setItem(const DrawItem& item)
{
    Shader::setItem(item);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.model"), 1, GL_TRUE, item.modelTransform.getValues());
}
    
void WireframeShader::setCamera(Camera* camera)
{
    Shader::setCamera(camera);
    ProgramBinding binding(this);

    glUniformMatrix4fv(getUniformLocation("uTransforms.view"), 1, GL_TRUE, camera->getViewMatrix().getValues());
    glUniformMatrix4fv(getUniformLocation("uTransforms.proj"), 1, GL_TRUE, camera->getProjectionMatrix().getValues());
}

void WireframeShader::setMaterial(const Material* material)
{
    if (!material || material->materialType != MaterialType::WIREFRAME)
    {
        throw IllegalArgumentException("WireframeShader requires a WireframeMaterial");
    }

    Shader::setMaterial(material);
    ProgramBinding binding(this);
    const WireframeMaterial* wfMat = static_cast<const WireframeMaterial*>(material);
    
    // Color
    Color color = wfMat->color;
    glUniform4f(getUniformLocation("uColor"), color.red(), color.green(), color.blue(), color.alpha());
}

