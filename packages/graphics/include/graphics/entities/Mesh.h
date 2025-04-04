#pragma once
#include <graphics/entities/pointers/MeshPtr.h>
#include <graphics/entities/Entity.h>
#include <graphics/geometry/pointers/GeometryPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>

/**
 * A triangulated polyface mesh
 * @author Nathaniel Rex
 */
class Mesh : public Entity
{
public:

	/**
	 * Constructs a new mesh instance
	 * @param geometry Geometry whose points are expected to form a series of triangles
	 * @param material Material
	 */
	static MeshPtr create(GeometryPtr geometry, MaterialPtr material);

private:

	/**
	 * Geometry. The points of this geometry are assumed to form a series of triangles.
	 */
	GeometryPtr _geometry;

	/**
	 * Material
	 */
	MaterialPtr _material;

	/**
	 * Constructor
	 * @param geometry Geometry whose points are expected to form a series of triangles.
	 * @param material Material
	 */
	Mesh(GeometryPtr geometry, MaterialPtr);
};