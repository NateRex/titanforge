#include <graphics/core/renderer/modes/RenderMode.h>
#include <graphics/core/renderer/modes/MaterialMode.h>
#include <graphics/core/renderer/modes/VertexNormalMode.h>

const RenderModePtr RenderMode::MATERIAL = std::make_shared<MaterialRenderMode>();
const RenderModePtr RenderMode::VERTEX_NORMALS = std::make_shared<VertexNormalRenderMode>();
