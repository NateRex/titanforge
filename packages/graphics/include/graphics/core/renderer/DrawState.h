#pragma once
#include <graphics/core/shaders/ShaderId.h>
#include <graphics/core/renderer/RenderModes.h>
#include <graphics/core/Color.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <vector>

class Material;
class Light;
class Camera;
class TextureCube;
class PostProcessMaterial;
class GeometryBuffer;
class InstanceBuffer;

/**
 * Maximum number of lights that can be applied to a scene (not including ambient lighting)
 */
constexpr unsigned int MAX_LIGHTS = 16;

/**
 * Maximum number of lights capable of producing shadows modeled using 2D textures. This covers directional
 * lights and spotlights.
 */
constexpr unsigned int MAX_SHADOW_MAPS_2D = 8;

/**
 * Maximum number of lights capable of producing shadows modeled using cube textures. This covers point lights.
 */
constexpr unsigned int MAX_SHADOW_MAPS_CUBE = 4;

/**
 * Maximum number of lights capable of producing shadows, in total
 */
constexpr unsigned int MAX_SHADOW_LIGHTS = MAX_SHADOW_MAPS_2D + MAX_SHADOW_MAPS_CUBE;

/**
 * First texture unit slot reserved for shadow maps
 */
constexpr unsigned int FIRST_SHADOW_TEXTURE_UNIT = 4;

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
	 * Geometry buffer, used for regular item draws. A null value implies that this is an instanced draw.
	 */
	GeometryBuffer* geometryBuffer = nullptr;

	/**
	 * Instance buffer, used for instanced items. A null value implies that this is a regular draw.
	 */
	InstanceBuffer* instanceBuffer = nullptr;

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

	/**
	 * @return The buffer containing either the ordinary or instanced geometry of this item
	 */
	const GeometryBuffer* getBuffer() const;

    /**
	 * Submits the prepared ordinary or instanced geometry to the shader. It is assumed that the shader is currently active, and
	 * that all uniforms have already been set.
	 */
    void drawGeometry() const;
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

	/**
	 * Shadow map texture, prepared by a shadow pass. Absent for lights that do not produce shadows.
	 */
    TexturePtr shadowMap;

	/**
	 * Index within the shadow texture type's sampler partition. Will be -1 for lights that do not cast shadows.
	 */
    int shadowIndex = -1;

	/**
	 * Transformation matrix used to determine shadows. Absent for lights that do not produce shadows.
	 */
    Matrix4 shadowTransform = Matrix4::IDENTITY;
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
	 * Linear RGB base color with unencoded alpha to blend with the sampled texture color
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
