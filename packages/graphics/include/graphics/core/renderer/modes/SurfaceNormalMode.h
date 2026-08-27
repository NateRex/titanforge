#pragma once
#include <graphics/core/renderer/modes/RenderMode.h>

/**
 * Rendering mode used to visualize world-space surface normals encoded as RGB colors
 * @author Nathaniel Rex
 */
class SurfaceNormalRenderMode final : public RenderMode
{
public:

	void apply(RenderItem& item) const override;
    
};