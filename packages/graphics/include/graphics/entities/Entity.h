#pragma once
#include <graphics/geometry/Geometry.h>
#include <math/Matrix3.h>

using EntityPtr = std::shared_ptr<class Entity>;

/**
 * Base class for all entities that can be added to the scene. This class wraps geometry and material needed for
 * rendering. An entity can have one or more children, whose position, rotation, and scaling will be relative to
 * its parent.
 * @author Nathaniel Rex
 */
class Entity
{

public:

	/**
	 * Constructs a new entity instance
	 * @param geometry Geometry
	 */
	static EntityPtr create(GeometryPtr geometry);

private:

	/**
	 * Geometry
	 */
	const GeometryPtr _geometry;

	/**
	 * World position
	 */
	Vector3 _position;

	/**
	 * World rotation
	 */
	Matrix3 _rotation;

	/**
	 * World scaling
	 */
	Vector3 _scale;

	/**
	 * Constructor
	 * @param geometry Geometry
	 */
	Entity(GeometryPtr geometry);
};