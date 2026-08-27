#pragma once
#include <graphics/core/renderer/modes/RenderMode.h>

/**
 * Rendering mode used to visualize world-space vertex normals
 * @author Nathaniel Rex
 */
class VertexNormalRenderMode final : public RenderMode
{
public:

	void apply(RenderItem& item) const override;
    
};