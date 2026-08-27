#include <graphics/core/shaders/NormalsShader.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/cameras/Camera.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

namespace
{
constexpr const char* NORMALS_VERTEX = R"(
    #version 330 core

    layout (location = 0) in vec3 vert_Pos;
    layout (location = 1) in vec3 vert_Normal;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;
    uniform mat3 uNormal;

    out vec3 worldNormal;

    void main()
    {
        worldNormal = normalize(uNormal * vert_Normal);
        gl_Position = uProjection * uView * uModel * vec4(vert_Pos, 1.0);
    }
)";

constexpr const char* NORMALS_FRAGMENT = R"(
    #version 330 core

    in vec3 worldNormal;
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(normalize(worldNormal) * 0.5 + 0.5, 1.0);
    }
)";
}

NormalsShader::NormalsShader(): Shader("NormalsShader", NORMALS_VERTEX, NORMALS_FRAGMENT)
{
}

void NormalsShader::setItem(const RenderItem& item)
{
	ProgramBinding binding(this);
	glUniformMatrix4fv(getUniformLocation("uModel"), 1, GL_TRUE, item.modelTransform.getValues());
	glUniformMatrix3fv(getUniformLocation("uNormal"), 1, GL_TRUE, item.normalTransform.getValues());
}

void NormalsShader::setCamera(Camera* camera)
{
	if (!camera)
	{
		throw IllegalArgumentException("NormalsShader requires a camera");
	}

	ProgramBinding binding(this);
	const Matrix4 view = camera->getViewMatrix();
	const Matrix4 projection = camera->getProjectionMatrix();
	glUniformMatrix4fv(getUniformLocation("uView"), 1, GL_TRUE, view.getValues());
	glUniformMatrix4fv(getUniformLocation("uProjection"), 1, GL_TRUE, projection.getValues());
}
