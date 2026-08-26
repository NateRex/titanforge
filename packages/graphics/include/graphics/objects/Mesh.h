#pragma once
#include <graphics/objects/pointers/MeshPtr.h>
#include <graphics/objects/Renderable.h>

/**
 * A triangulated polyface mesh
 * @author Nathaniel Rex
 */
class Mesh : public Renderable
{
public:

	/**
	 * Constructs a new mesh instance
	 * @param geometry Geometry whose points are expected to form a series of triangles
	 * @param material Material
	 * @return The new mesh instance
	 */
	static MeshPtr create(GeometryPtr geometry, MaterialPtr material)
	{
		return std::shared_ptr<Mesh>(new Mesh(geometry, material));
	}

protected:

	/**
	 * Constructor
	 * @param geometry Geometry whose points are expected to form a series of triangles.
	 * @param material Material
	 */
	Mesh(GeometryPtr geometry, MaterialPtr material): Renderable(geometry, material) {}
};
