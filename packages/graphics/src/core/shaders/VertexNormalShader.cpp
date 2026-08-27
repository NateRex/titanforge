#include <graphics/core/shaders/VertexNormalShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Source code for the normals vertex shader
 */
constexpr const char* NORMALS_VERTEX = R"(
    #version 330 core

    layout (location = 0) in vec3 vert_Pos;
    layout (location = 1) in vec3 vert_Normal;

    uniform mat4 uModel;
    uniform mat3 uNormal;

    out vec3 worldNormal;

    void main()
    {
        // Compute world position
        gl_Position = uModel * vec4(vert_Pos, 1.0);

        // Compute world normal
        worldNormal = normalize(uNormal * vert_Normal);
    }
)";

/**
 * Source code for the normals geometry shader
 */
constexpr const char* NORMALS_GEOMETRY = R"(
    #version 330 core

    layout(triangles) in;
    layout(line_strip, max_vertices = 6) out;

    in vec3 worldNormal[];

    uniform mat4 uView;
    uniform mat4 uProjection;

    const float LINE_LENGTH = 0.4;

    void emitNormal(mat4 viewProjection, int i)
    {
        gl_Position = viewProjection * gl_in[i].gl_Position;
        EmitVertex();

        gl_Position = viewProjection * (gl_in[i].gl_Position + vec4(worldNormal[i] * LINE_LENGTH, 0.0));
        EmitVertex();
        EndPrimitive();
    }

    void main()
    {
        mat4 viewProjection = uProjection * uView;
        emitNormal(viewProjection, 0);
        emitNormal(viewProjection, 1);
        emitNormal(viewProjection, 2);
    }
)";

/**
 * Source code for the normals fragment shader
 */
constexpr const char* NORMALS_FRAGMENT = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
)";

VertexNormalShader::VertexNormalShader(): Shader("VertexNormalShader", NORMALS_VERTEX, NORMALS_GEOMETRY, NORMALS_FRAGMENT)
{
}

void VertexNormalShader::setItem(const RenderItem& item)
{
	ProgramBinding binding(this);
	glUniformMatrix4fv(getUniformLocation("uModel"), 1, GL_TRUE, item.modelTransform.getValues());
	glUniformMatrix3fv(getUniformLocation("uNormal"), 1, GL_TRUE, item.normalTransform.getValues());
}

void VertexNormalShader::setCamera(Camera* camera)
{
	if (!camera)
	{
		throw IllegalArgumentException("VertexNormalShader requires a camera");
	}

	ProgramBinding binding(this);
	const Matrix4 view = camera->getViewMatrix();
	const Matrix4 projection = camera->getProjectionMatrix();
	glUniformMatrix4fv(getUniformLocation("uView"), 1, GL_TRUE, view.getValues());
	glUniformMatrix4fv(getUniformLocation("uProjection"), 1, GL_TRUE, projection.getValues());
}
