#pragma once
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <graphics/core/Color.h>

/**
 * Describes how face-culling should be applied, if at all
 * @author Nathaniel Rex
 */
enum class CullingMode
{
	/**
	 * No face culling will be applied. Both sides to every facet will be drawn.
	 */
	NONE,

	/**
	 * Front-facing facets will be culled
	 */
	FRONT,

	/**
	 * Back-facing facets will be culled
	 */
	BACK
};

/**
 * Describes the function to use when assessing if a fragment will pass the depth test
 * @author Nathaniel Rex
 */
enum class DepthFunction
{
	/**
	 * The test always passes
	 */
	ALWAYS,

	/**
	 * The test never passes
	 */
	NEVER,

	/**
	 * Passes if the fragment's value is less than the stored value
	 */
	LESS,

	/**
	 * Passes if the fragment's value is less than or equal to the stored value
	 */
	LESS_OR_EQUAL_TO,

	/**
	 * Passes if the fragment's value is greater than the stored value
	 */
	GREATER,

	/**
	 * Passes if the fragment's value is greater than or equal to the stored value
	 */
	GREATER_OR_EQUAL_TO,

	/**
	 * Passes if the fragment's value is not equal to the stored value
	 */
	NOT_EQUAL,

	/**
	 * Passes if the fragment's value is equal to the stored value
	 */
	EQUAL
};

/**
 * Describes the units used to measture the size of primitives (such as points and lines)
 * @author Nathaniel Rex
 */
enum class PrimitiveSizeUnits
{
    /**
     * Size is measured in screen pixels
     */
    PIXELS,

    /**
     * Size is measured in world-space units
     */
    WORLD
};

/**
 * Base class for all materials that define the appearance of objects in a scene.
 * @author Nathaniel Rex
 */
class Material
{
public:

	/**
	 * Material type
	 */
	const MaterialType materialType;

	/**
	 * Base color. Defaults to solid white.
	 */
	Color color = Color::WHITE;

	/**
	 * Texture. Can be null.
	 */
	TexturePtr texture = nullptr;

	/**
	 * Face-culling mode. Defaults to CullingMode::BACK.
	 */
	CullingMode cullingMode = CullingMode::BACK;

	/**
	 * Depth test function. Defaults to DepthFunction::LESS.
	 */
	DepthFunction depthFunction = DepthFunction::LESS;

	/**
	 * Boolean flag that, when true, enables depth writes. Defaults to true.
	 */
	bool depthWrite = true;

	/**
	 * Destructor
	 */
	virtual ~Material() = default;

	/**
	 * @return True if this material represents a transparent or semi-transparent object. Returns false otherwise.
	 */
	virtual bool isTransparent() const { return false; }

	/**
	 * @return True if this material applies to background scenery. Returns false otherwise. 
	 */
	virtual bool isBackground() const { return false; }

protected:

	/**
	 * Constructor
	 * @param type Material type
	 */
	Material(MaterialType type);
};