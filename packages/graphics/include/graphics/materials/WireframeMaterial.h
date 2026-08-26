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
     * Line width, in world-space units. Defaults to 0.05.
     */
    float lineWidth = 0.05f;

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
    WireframeMaterial(): Material(MaterialType::WIREFRAME) {}
};