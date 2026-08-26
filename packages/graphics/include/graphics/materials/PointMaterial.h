#pragma once
#include <graphics/materials/pointers/PointMaterialPtr.h>
#include <graphics/materials/Material.h>

/**
 * Describes how a point's size is measured
 * @author Nathaniel Rex
 */
enum class PointSizeUnits
{
    /**
     * Point diameter is measured in screen pixels
     */
    PIXELS,

    /**
     * Point diameter is measured in world-space units
     */
    WORLD
};

/**
 * Material used to render point primitives
 * @author Nathaniel Rex
 */
class PointMaterial : public Material
{
public:

    /**
     * Controls how the point size is interpreted. Defaults to PointSizeUnits::PIXELS.
     */
    PointSizeUnits sizeUnits = PointSizeUnits::PIXELS;

    /**
     * Point diameter. The units for this value are determine via PointMaterial::sizeUnits. Defaults to 6 pixels.
     */
    float size = 6.f;

    /**
     * Creates a point material
     * @return The new material instance
     */
    static PointMaterialPtr create();

private:

    /**
     * Constructor
     */
    PointMaterial();
};
