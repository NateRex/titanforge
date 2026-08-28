#pragma once
#include <graphics/core/shaders/ShaderId.h>
#include <graphics/core/renderer/RenderModes.h>
#include <graphics/core/Color.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <vector>

class Geometry;
class Material;
class Light;
class Camera;
class TextureCube;
class PostProcessMaterial;

/**
 * Determines when an item is drawn relative to other items in a pass
 * @author Nathaniel Rex
 */
enum class DrawLayer
{
    /**
     * Fully-opaque items where no surface blending is necessary
     */
	OPAQUE,

    /**
     * Items that make up the background of a scene (e.g., a skybox)
     */
	BACKGROUND,

    /**
     * Transparent items, where item ordering and surface blending matters
     */
	TRANSPARENT
};

/**
 * A flattened, render-ready description of an object. The fields of this object are updated during scene traversal, as well
 * as resolution of the current render pass settings.
 * @author Nathaniel Rex
 */
struct DrawItem {

	/**
	 * Item drawing variant for a specific to a rendering mode
	 */
	struct Variant {

		/**
		 * Rendering mode
		 */
		RenderModes mode;

		/**
		 * ID of the shader to use
		 */
		ShaderId shader;

		/**
		 * The layer the item belongs to
		 */
		DrawLayer layer;
	};

	/**
	 * Geometry being rendered
	 */
	Geometry* geometry = nullptr;

	/**
	 * Material to apply
	 */
	Material* material = nullptr;

	/**
	 * Local-to-world transformation for vertices, accounting for all parent entities of the mesh.
	 */
	Matrix4 modelTransform = Matrix4::IDENTITY;

	/**
	 * Local-to-world transformation for vertex normals, accounting for all parent entities of the mesh.
	 */
	Matrix3 normalTransform = Matrix3::IDENTITY;

	/**
	 * Variants for each type of rendering mode supported by this item
	 */
	std::vector<Variant> variants;

	/**
	 * Fetches the drawing variant for a given rendering mode
	 * @param mode Rendering mode
	 * @return The variant, or null if no variant exists for that rendering mode
	 */
	const Variant* variant(RenderModes mode) const;
};

/**
 * A flattened, render-ready description of a light instance.
 */
struct LightInstance {

	/**
	 * Light providing this instance's color and attenuation properties.
	 */
	Light* light = nullptr;

	/**
	 * Position in world space.
	 */
	Vector3 position = Vector3::ZERO;

	/**
	 * Direction in which the light's rays travel, in world space.
	 */
	Vector3 direction = Vector3::ZERO;
};

/**
 * Image-based surroundings available to shaders during a render pass
 * @author Nathaniel Rex
 */
struct Environment {

	/**
	 * Image texture to sample from
	 */
	TextureCube* texture = nullptr;

	/**
	 * Base color to blend with the sampled texture color
	 */
	Color color = Color::WHITE;

	/**
	 * Linear multiplier applied to the sampled texture color
	 */
	float intensity = 1.f;

	/**
	 * Rotation of the background texture around the world up axis, in radians
	 */
	float rotation = 0.f;

	/**
	 * Explicit cubemap mip level. Values above zero provide blur when mipmaps exist.
	 */
	float lod = 0.f;
};

/**
 * Aggregation of all draw data required for a single render pass
 * @author Nathaniel rex
 */
struct DrawState {

	/**
	 * Environment used for image-based reflection and refraction
	 */
	Environment environment;

	/**
	 * Lights affecting this render pass. The shader consumes up to its supported
	 * maximum; keeping the full list here allows selection policies to be added
	 * without constraining the scene itself.
	 */
	std::vector<LightInstance> lights;

	/**
	 * Full-screen effects to apply after all scene items have been drawn, in scene traversal order
	 */
	std::vector<PostProcessMaterial*> postProcessing;

	/**
	 * The items to be drawn this frame
	 */
	std::vector<DrawItem> items;
};
