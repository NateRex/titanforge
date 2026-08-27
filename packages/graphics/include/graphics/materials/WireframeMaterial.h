#pragma once
#include <graphics/materials/pointers/WireframeMaterialPtr.h>
#include <graphics/materials/Material.h>

/**
 * A material used to render a mesh as a wireframe
 * @author Nathaniel Rex
 */
class WireframeMaterial : public Material
{
public:

    /**
     * Controls how the line width is interpreted. Defaults to PrimitiveSizeUnits::PIXELS.
     */
    PrimitiveSizeUnits lineWidthUnits = PrimitiveSizeUnits::PIXELS;

    /**
     * Line width. The units are determined by WireframeMaterial::lineWidthUnits. Defaults to 1 pixels.
     */
    float lineWidth = 1.f;

    /**
     * Creates a new wireframe material
     * @return The new wireframe material
     */
    static WireframeMaterialPtr create()
    {
        return std::shared_ptr<WireframeMaterial>(new WireframeMaterial());
    }

private:

    /**
     * Constructor
     */
    WireframeMaterial(): Material(MaterialType::WIREFRAME, ShaderId::WIREFRAME)
    {
        cullingMode = CullingMode::NONE;
    }
};
