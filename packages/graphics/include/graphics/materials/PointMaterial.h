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
     * Point size in pixels
     */
    float size = 5.f;

    /**
     * Boolean flag that, when true, causes the point to appear with fixed sizing regardless of where the camera is. Defaults to false.
     */
    bool fixedSize = false;

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