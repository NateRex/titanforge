#include <graphics/core/renderer/DrawState.h>
#include <algorithm>

const DrawItem::Variant* DrawItem::variant(RenderModes mode) const
{
    const auto it = std::find_if(variants.begin(), variants.end(), [mode](const DrawItem::Variant& candidate) {
        return candidate.mode == mode;
    });

    return it != variants.end() ? &*it : nullptr;
}