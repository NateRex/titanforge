#pragma once
#include <graphics/core/shaders/FullScreenShader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

/**
 * Shader used to handle post-processing materials
 * @author Nathaniel Rex
 */
class PostProcessShader : public FullScreenShader
{
public:

	/**
	 * Constructs a new PostProcessShader instance
	 * @return The new PostProcessShader instance
	 */
	static ShaderPtr create()
	{
		return std::shared_ptr<PostProcessShader>(new PostProcessShader());
	}

	void setMaterial(const Material* material) override;

private:
	
	/**
	 * Constructor
	 */
	PostProcessShader();
};
