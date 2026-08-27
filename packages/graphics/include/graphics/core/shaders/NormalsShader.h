#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

/**
 * Shader used by RenderMode::SURFACE_NORMALS to visualize world-space surface normals.
 */
class NormalsShader : public Shader
{
public:
	static ShaderPtr create()
	{
		return std::shared_ptr<NormalsShader>(new NormalsShader());
	}

	void setItem(const RenderItem& item) override;
	void setCamera(Camera* camera) override;

private:

	NormalsShader();
};
