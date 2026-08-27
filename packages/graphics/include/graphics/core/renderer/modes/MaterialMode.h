#pragma once
#include <graphics/core/renderer/modes/RenderMode.h>

/**
 * Rendering mode that results in default rendering using each item's material
 * @author Nathaniel Rex
 */
class MaterialRenderMode final : public RenderMode
{
public:

	void apply(RenderItem& item) const override;
    
};