#include <graphics/core/renderer/modes/MaterialMode.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/materials/Material.h>

void MaterialRenderMode::apply(RenderItem& item) const
{
    // Items are always visible in material rendering
    item.visible = true;

    // Resolve shader
    item.shader = item.material->shaderId();

    // Resolve item layer
    item.layer = RenderLayer::OPAQUE;
    if (item.material->isTransparent())
    {
        item.layer = RenderLayer::TRANSPARENT;
    }
    else if (item.material->isBackground())
    {
        item.layer = RenderLayer::BACKGROUND;
    }
}