#pragma once
#include <graphics/objects/pointers/MeshPtr.h>
#include <graphics/core/Entity.h>
#include <graphics/geometry/pointers/GeometryPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/materials/pointers/MeshMaterialPtr.h>

/**
 * A triangulated polyface mesh
 * @author Nathaniel Rex
 */
class Mesh : public Entity
{
public:

	/**
	 * Geometry. The points of this geometry are assumed to form a series of triangles.
	 */
	GeometryPtr geometry;

	/**
	 * Material
	 */
	MaterialPtr material;

	/**
	 * Constructs a new mesh instance
	 * @param geometry Geometry whose points are expected to form a series of triangles
	 * @param material Material
	 * @return The new mesh instance
	 */
	static MeshPtr create(GeometryPtr geometry, MaterialPtr material);

	void traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

	/**
	 * Constructor
	 * @param entityType Entity type. Used by child classes to override the default value of EntityType::MESH.
	 * @param geometry Geometry whose points are expected to form a series of triangles.
	 * @param material Material
	 */
	Mesh(EntityType entityType, GeometryPtr geometry, MaterialPtr material);
};
