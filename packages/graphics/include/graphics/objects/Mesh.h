#pragma once
#include <graphics/objects/pointers/MeshPtr.h>
#include <graphics/core/Entity.h>
#include <graphics/geometry/pointers/GeometryPtr.h>
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
	MeshMaterialPtr material;

	/**
	 * Constructs a new mesh instance
	 * @param geometry Geometry whose points are expected to form a series of triangles
	 * @param material Mesh material
	 * @return The new mesh instance
	 */
	static MeshPtr create(GeometryPtr geometry, MeshMaterialPtr material);

private:

	/**
	 * Constructor
	 * @param geometry Geometry whose points are expected to form a series of triangles.
	 * @param material Mesh material
	 */
	Mesh(GeometryPtr geometry, MeshMaterialPtr);
};