#pragma once
#include <graphics/core/shaders/MeshShader.h>

/**
 * Shader used to handle instanced meshes, where each instance shares the same geometry, material, and model matrix.
 * @author Nathaniel Rex
 */
class InstancedMeshShader : public MeshShader
{
public:

	/**
	 * Constructs a new InstancedMeshShader instance
	 */
	static ShaderPtr create()
	{
		return std::shared_ptr<InstancedMeshShader>(new InstancedMeshShader());
	}

private:

	/**
	 * Constructor
	 */
	InstancedMeshShader();

};
