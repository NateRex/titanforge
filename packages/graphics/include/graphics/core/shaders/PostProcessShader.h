#pragma once
#include <graphics/core/shaders/pointers/PostProcessShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Shader used to handle post-processing materials
 * @author Nathaniel Rex
 */
class PostProcessShader : public Shader
{
public:

	/**
	 * Constructs a new PostProcessShader instance. This should typically only be done once, by the shader manager.
	 * @return The new PostProcessShader instance
	 */
	static PostProcessShaderPtr create()
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
