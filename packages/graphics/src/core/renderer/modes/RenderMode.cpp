#include <graphics/core/renderer/modes/RenderMode.h>
#include <graphics/core/renderer/modes/MaterialMode.h>
#include <graphics/core/renderer/modes/SurfaceNormalMode.h>

const RenderModePtr RenderMode::MATERIAL = std::make_shared<MaterialRenderMode>();
const RenderModePtr RenderMode::SURFACE_NORMALS = std::make_shared<SurfaceNormalRenderMode>();
