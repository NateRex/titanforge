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
		// This shader only ever receives one triangle with three vertices. Using the vertex ID, we can
		// generate positions (0, 0), (2, 0), (2, 2), which form the coordinates of an oversized triangle
		// bigger than the screen
		vec2 position = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));
		frag_TexCoord = position;

		// Transform position to clip-space screen coordinates. We end up with positions (-1, -1), (3, -1), (3, 3)
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

		// Multiply base color by exposure. Values greater than 1 brighten the image, while values less than 1 darken it.
		vec3 color = source.rgb * uMaterial.exposure;

		// Reduce RGB to one perceived-brightness value. This is a weighted average rather
		// than a plain average because human vision notices green most, red next, and blue
		// least; the three weights add to 1, preserving neutral gray brightness.
		float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));

		// Make a gray color from luminance, then blend toward the original color. Saturation
		// 0 chooses gray, 1 chooses the original, and values in between mix the two.
		color = mix(vec3(luminance), color, uMaterial.saturation);
		
		// Adjust contrast around middle gray (0.5): subtracting 0.5 measures each channel's
		// distance from gray, contrast scales that distance, and adding 0.5 recenters it.
		// Thus 1 changes nothing, above 1 spreads dark/light values apart, and below 1
		// pulls them together.
		color = (color - 0.5) * uMaterial.contrast + 0.5;

		// Reattach the source opacity, then multiply RGBA by the material color. Each tint
		// channel acts like a filter: 1 preserves that channel and 0 removes it.
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

	void setMaterial(const MaterialPtr material) override;

private:
	
	/**
	 * Constructor
	 */
	PostProcessShader();
};
