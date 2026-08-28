#pragma once
#include <graphics/geometry/pointers/GeometryPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/core/Entity.h>
#include <graphics/core/shaders/ShaderId.h>

struct DrawItem;

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

    void traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

    /**
     * Constructor
     * @param geometry Geometry
     * @param material Material
     */
    Renderable(GeometryPtr geometry, MaterialPtr material): geometry(geometry), material(material) {}

    /**
     * Configures the buffer and variants of an item representing this entity in a render state
     * @param item Draw item to modify
     */
    virtual void configureDrawItem(DrawItem& item);

    /**
     * Adds a material variant to an item representing this entity in a render state.
     * @param item Draw item to modify
     * @param shader ID of the shader that should be used to draw the item
     */
    void addMaterialVariant(DrawItem& item, ShaderId shader);

    /**
     * If supported, add a vertex normal variant to an item representing this entity in a render state. If 
     * vertex normal visualization is not supported by this entity, this method does nothing.
     * @param item Draw item to modify
     */
    void addVertexNormalVariant(DrawItem& item);
};
