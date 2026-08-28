#include <graphics/core/shaders/InstancedMeshShader.h>

/**
 * Vertex shader code for the instanced mesh shader
 */
constexpr const char* INSTANCED_MESH_VERTEX = R"(
	#version 330 core

	struct Transforms { mat4 model; mat4 view; mat4 proj; mat3 normal; };
	layout (location = 0) in vec3 vert_Pos;
	layout (location = 1) in vec3 vert_Normal;
	layout (location = 2) in vec4 vert_VertexColor;
	layout (location = 3) in vec2 vert_TexCoord;
	layout (location = 4) in mat4 instance_Model;

	uniform Transforms uTransforms;
	out vec3 frag_Pos;
	out vec3 frag_Normal;
	out vec4 frag_VertexColor;
	out vec2 frag_TexCoord;

	void main()
	{
		mat4 model = uTransforms.model * instance_Model;
		vec4 worldPos = model * vec4(vert_Pos, 1.0);
		frag_Pos = worldPos.xyz;
		frag_Normal = uTransforms.normal * transpose(inverse(mat3(instance_Model))) * vert_Normal;
		frag_VertexColor = vert_VertexColor;
		frag_TexCoord = vert_TexCoord;
		gl_Position = uTransforms.proj * uTransforms.view * worldPos;
	}
)";

InstancedMeshShader::InstancedMeshShader(): MeshShader("InstancedMeshShader", INSTANCED_MESH_VERTEX, nullptr, nullptr)
{
	
}
