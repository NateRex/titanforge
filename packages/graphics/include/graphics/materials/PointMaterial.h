#pragma once
#include <graphics/materials/pointers/PointMaterialPtr.h>
#include <graphics/materials/Material.h>

/**
 * Material used to render point primitives
 * @author Nathaniel Rex
 */
class PointMaterial : public Material
{
public:

    /**
     * Controls how the point size is interpreted. Defaults to PrimitiveSizeUnits::PIXELS.
     */
    PrimitiveSizeUnits sizeUnits = PrimitiveSizeUnits::PIXELS;

    /**
     * Point diameter. The units for this value are determine via PointMaterial::sizeUnits. Defaults to 6 pixels.
     */
    float size = 6.f;

    /**
     * Creates a point material
     * @return The new material instance
     */
    static PointMaterialPtr create()
    {
        return std::shared_ptr<PointMaterial>(new PointMaterial());
    }

private:

    /**
     * Constructor
     */
    PointMaterial() : Material(MaterialType::POINT) {
        cullingMode = CullingMode::NONE;
    }
};
