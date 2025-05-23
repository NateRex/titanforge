#pragma once
#include <graphics/entities/EntityType.h>
#include <graphics/entities/pointers/EntityPtr.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <vector>

/**
 * Base class for all entities that can be added to the scene. Contains a set of properties and methods for manipulating
 * the object in 3D space. An entity can have one or more children, whose position, rotation, and scaling will be relative
 * to its parent.
 * @author Nathaniel Rex
 */
class Entity
{
public:

	friend class Renderer;

	/**
	 * Entity type
	 */
	const EntityType type;

	/**
	 * Destructor
	 */
	virtual ~Entity() = default;

	/**
	 * @return The position of this entity relative to its parent.
	 */
	Vector3 getPosition() const;

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
	 * @return The 3x3 matrix representing the rotation of this entity relative to its parent
	 */
	Matrix3 getRotation() const;

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
	 * @return The scaling of this entity relative to its parent across the x, y, and z axes
	 */
	Vector3 getScaling() const;

	/**
	 * Sets the scaling of this entity relative to its parent.
	 * @param x Scaling in the x direction
	 * @param y Scaling in the y direction
	 * @param z Scaling in the z direction
	 */
	virtual void setScaling(float x, float y, float z);

	/**
	 * Applies additional scaling to this entity relative to its parent.
	 * @param x Scaling in the x direction
	 * @param y Scaling in the y direction
	 * @param z Scaling in the z direction
	 */
	virtual void addScaling(float x, float y, float z);

	/**
	 * @return A matrix representing the transformation of this entity from local space to the reference frame of
	 * it's direct parent
	 */
	Matrix4 getMatrix();

	/**
	 * @return The parent of this entity. Can be null.
	 */
	Entity* getParent() const;

	/**
	 * @return The number of children this entity has
	 */
	unsigned int getNumberOfChildren() const;

	/**
	 * Adds a child to this entity.
	 * @param child The child entity
	 */
	void add(EntityPtr entity);
	
	/**
	 * Removes a child from this entity.
	 * @param child The child entity
	 */
	void remove(EntityPtr entity);

protected:

	/**
	 * Position
	 */
	Vector3 _position;

	/**
	 * Rotation
	 */
	Matrix3 _rotation;

	/**
	 * Scaling
	 */
	Vector3 _scale;

	/**
	 * Cached matrix representing the transformation of this entity from local to parent space. This matrix is lazily updated
	 * when requested.
	 */
	Matrix4 _transform;

	/**
	 * Boolean flag that, when true, indicates that one or more values representing the transformation of this entity have changed,
	 * and thus the transformation matrix needs to be re-computed.
	 */
	bool _transformNeedsUpdate = true;

	/**
	 * Parent entity. Is null by default.
	 */
	Entity* _parent = nullptr;

	/**
	 * Child entities. Can be empty.
	 */
	std::vector<EntityPtr> _children;

	/**
	 * Constructor
	 * @param type Entity type
	 */
	Entity(EntityType type);

	/**
	 * Checks whether or not the cached transformation matrix for this entity is out-of-date, and if so, updates it
	 */
	void updateTransform();
};