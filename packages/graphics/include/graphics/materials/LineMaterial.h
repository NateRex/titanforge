#pragma once
#include <graphics/materials/pointers/LineMaterialPtr.h>
#include <graphics/materials/Material.h>

/**
 * Material used to render line primitives
 * @author Nathaniel Rex
 */
class LineMaterial : public Material
{
public:

    /**
     * Controls how the line width is interpreted. Defaults to PrimitiveSizeUnits::PIXELS.
     */
    PrimitiveSizeUnits widthUnits = PrimitiveSizeUnits::PIXELS;

    /**
     * Line width. The units for this value are determine via LineMaterial::widthUnits. Defaults to 3 pixels.
     */
    float width = 3.f;

    /**
     * Creates a line material
     * @return The new material instance
     */
    static LineMaterialPtr create();

private:

    /**
     * Constructor
     */
    LineMaterial();
};