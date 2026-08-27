#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

/**
 * Shader used to visualize vertex normals
 * @author Nathaniel Rex
 */
class VertexNormalShader : public Shader
{
public:

	/**
	 * Constructs a new VertexNormalShader instance
	 * @return The new shader instance
	 */
	static ShaderPtr create()
	{
		return std::shared_ptr<VertexNormalShader>(new VertexNormalShader());
	}

	void setItem(const RenderItem& item) override;
	void setCamera(Camera* camera) override;

private:

	/**
	 * Constructor
	 */
	VertexNormalShader();
};
