#include <graphics/core/renderer/modes/SurfaceNormalMode.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>

void SurfaceNormalRenderMode::apply(RenderItem& item) const
{
    // In surface normal mode, items are only visible if they contain surface normals
    const MaterialType materialType = item.material->materialType;
    item.visible = item.geometry->getAttributes().normals &&
        (materialType == MaterialType::MESH || materialType == MaterialType::WIREFRAME);

    // Resolve shader
    item.shader = ShaderId::NORMALS;

    // All items can be considered opaque, since we are only rendering the surface normals this pass
    item.layer = RenderLayer::OPAQUE;
}