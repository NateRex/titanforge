#pragma once
#include <graphics/entities/EntityType.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>

/**
 * Base class for all entities that can be added to the scene. Contains a set of properties and methods for manipulating
 * the object in 3D space. An entity can have one or more children, whose position, rotation, and scaling will be relative
 * to its parent.
 * @author Nathaniel Rex
 */
class Entity
{

public:

	/**
	 * Position
	 */
	Vector3 position;

	/**
	 * Rotation
	 */
	Matrix3 rotation;

	/**
	 * Scaling
	 */
	Vector3 scale;

	/**
	 * Sets the position of this entity relative to its parent.
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param z Z coordinate
	 */
	void setPosition(float x, float y, float z);

	/**
	 * Applies additional translation of this entity relative to its parent
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param z Z coordinate
	 */
	void addPosition(float x, float y, float z);

	/**
	 * Sets the rotation of this entity relative to its parent, using a rotation matrix.
	 * @param rot Rotation matrix
	 */
	void setRotation(const Matrix3& rot);

	/**
	 * Applies additional rotation to this entity relative to its parent, using a rotation matrix.
	 */
	void addRotation(const Matrix3& rot);

	/**
	 * Sets the scaling of this entity relative to its parent.
	 * @param x Scaling in the x direction
	 * @param y Scaling in the y direction
	 * @param z Scaling in the z direction
	 */
	void setScaling(float x, float y, float z);

	/**
	 * Applies additional scaling to this entity relative to its parent.
	 * @param x Scaling in the x direction
	 * @param y Scaling in the y direction
	 * @param z Scaling in the z direction
	 */
	void addScaling(float x, float y, float z);

protected:

	/**
	 * Entity type
	 */
	const EntityType _type;

	/**
	 * Constructor
	 * @param type Entity type
	 */
	Entity(EntityType type);
};