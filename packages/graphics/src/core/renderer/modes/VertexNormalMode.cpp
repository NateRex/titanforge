#include <graphics/core/renderer/modes/VertexNormalMode.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>

void VertexNormalRenderMode::apply(RenderItem& item) const
{
    // In vertex normal mode, items are only visible if they contain vertex normals
    const MaterialType materialType = item.material->materialType;
    item.visible = item.geometry->getAttributes().normals &&
        (materialType == MaterialType::MESH || materialType == MaterialType::WIREFRAME);

    // Resolve shader
    item.shader = ShaderId::VERTEX_NORMALS;

    // All items can be considered opaque, since we are only rendering the vertex normals this pass
    item.layer = RenderLayer::OPAQUE;
}