#pragma once
#include <graphics/core/shaders/pointers/PostProcessShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Source code for the post-processing vertex shader
 */
constexpr const char* POST_PROCESS_VERTEX = R"(
	#version 330 core

	out vec2 frag_TexCoord;

	void main()
	{
		vec2 position = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));
		frag_TexCoord = position;
		gl_Position = vec4(position * 2.0 - 1.0, 0.0, 1.0);
	}
)";

/**
 * Source code for the post-processing fragment shader
 */
constexpr const char* POST_PROCESS_FRAGMENT = R"(
	#version 330 core

	struct Material {
		sampler2D texture;
		vec4 color;
		float exposure;
		float saturation;
		float contrast;
	};

	in vec2 frag_TexCoord;

	uniform Material uMaterial;

	out vec4 FragColor;

	void main()
	{
		vec4 source = texture(uMaterial.texture, frag_TexCoord);
		vec3 color = source.rgb * uMaterial.exposure;
		float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
		color = mix(vec3(luminance), color, uMaterial.saturation);
		color = (color - 0.5) * uMaterial.contrast + 0.5;
		FragColor = vec4(color, source.a) * uMaterial.color;
	}
)";

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
	static PostProcessShaderPtr create();

protected:

	void setMaterial(const MaterialPtr material) override;

private:
	
	/**
	 * Constructor
	 */
	PostProcessShader();
};
