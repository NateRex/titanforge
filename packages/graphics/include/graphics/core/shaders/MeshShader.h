#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

/**
 * Shader used to handle meshes and their materials
 * @author Nathaniel Rex
 */
class MeshShader : public Shader
{
public:

	/**
	 * Constructs a new MeshShader instance. This should typically only be done once, by the shader manager.
	 * @return The new MeshShader instance
	 */
	static ShaderPtr create()
	{
		return std::shared_ptr<MeshShader>(new MeshShader());
	}

	void setItem(const DrawItem& item) override;

	void setCamera(Camera* camera) override;

	void setMaterial(const Material* material) override;

	void setLighting(const std::vector<LightInstance>& lights) override;

	void setEnvironment(const Environment& environment) override;

	/**
     * Updates uniforms for this shader using the given model matrix
     * @param matrix Matrix representing the transformation from local to world space
     */
	void setModelMatrix(const Matrix4& matrix);

	/**
     * Updates the uniforms for this shader using the given view matrix
     * @param matrix Matrix representing the transformation from world to view space
     */
    void setViewMatrix(const Matrix4& matrix);

	/**
     * Updates the uniforms for this shader using the given projection matrix
     * @param matrix Matrix representing the transformation from view to clipping space
     */
    void setProjectionMatrix(const Matrix4& matrix);

	/**
     * Updates the uniforms for this shader using the given normal matrix. This matrix is typically used to
	 * transform vertex normals from local space to world space, without affecting scaling or translation.
     * @param matrix Matrix representing the transformation from local to world space for normal vectors
     */
    void setNormalMatrix(const Matrix3& matrix);

private:

	/**
	 * Constructor
	 */
	MeshShader();
};
