#pragma once
#include <graphics/entities/pointers/EntityPtr.h>
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
	 */
	static MeshPtr create(GeometryPtr geometry, MaterialPtr material);

private:

	/**
	 * Constructor
	 * @param geometry Geometry whose points are expected to form a series of triangles.
	 * @param material Material
	 */
	Mesh(GeometryPtr geometry, MaterialPtr);
};