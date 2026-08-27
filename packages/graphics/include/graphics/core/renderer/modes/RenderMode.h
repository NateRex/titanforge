#pragma once
#include <memory>

struct RenderItem;
using RenderModePtr = std::shared_ptr<const class RenderMode>;

/**
 * Strategy that decides how scene items are visualized in a render pass
 * @author Nathaniel Rex
 */
class RenderMode
{
public:

	/**
     * Default rendering using each item's material.
     */
	static const RenderModePtr MATERIAL;

	/**
     * World-space surface normals encoded as RGB colors
     */
	static const RenderModePtr SURFACE_NORMALS;

    /**
     * Destructor
     */
	virtual ~RenderMode() = default;

	/**
	 * Resolves all pass-specific properties for an item based on this rendering mode. Called once per item per pass.
     * @param item Item to modify in-place
	 */
	virtual void apply(RenderItem& item) const = 0;
};