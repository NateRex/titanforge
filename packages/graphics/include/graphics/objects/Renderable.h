#pragma once
#include <graphics/geometry/pointers/GeometryPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/core/Entity.h>

/**
 * A renderable represents anything consisting of geometry and material, capable of being visualized as part of a scene
 * @author Nathaniel Rex
 */
class Renderable : public Entity
{
public:

    /**
     * Geometry
     */
    GeometryPtr geometry;

	/**
	 * Material
	 */
	MaterialPtr material;

    void traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

    /**
     * Constructor
     * @param geometry Geometry
     * @param material Material
     */
    Renderable(GeometryPtr geometry, MaterialPtr material): geometry(geometry), material(material) {}
};