#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

/**
 * Shader used to visualize surface normals
 * @author Nathaniel Rex
 */
class NormalShader : public Shader
{
public:

	/**
	 * Constructs a new NormalShader instance
	 * @return The new shader instance
	 */
	static ShaderPtr create()
	{
		return std::shared_ptr<NormalShader>(new NormalShader());
	}

	void setItem(const RenderItem& item) override;
	void setCamera(Camera* camera) override;

private:

	/**
	 * Constructor
	 */
	NormalShader();
};
