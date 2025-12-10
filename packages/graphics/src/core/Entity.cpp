#include <graphics/core/Entity.h>
#include <math/Matrix4.h>
#include <algorithm>

Entity::Entity(EntityType type): type(type), _scale(1.f, 1.f, 1.f)
{

}

Vector3 Entity::getPosition() const
{
	return _position;
}

void Entity::setPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_transformNeedsUpdate = true;
}

void Entity::setPosition(const Vector3& v)
{
	_position.x = v.x;
	_position.y = v.y;
	_position.z = v.z;

	_transformNeedsUpdate = true;
}

void Entity::addPosition(float x, float y, float z)
{
	_position.x += x;
	_position.y += y;
	_position.z += z;

	_transformNeedsUpdate = true;
}

void Entity::addPosition(const Vector3& v)
{
	_position.x += v.x;
	_position.y += v.y;
	_position.z += v.z;

	_transformNeedsUpdate = true;
}

Matrix3 Entity::getRotation() const
{
	return _rotation;
}

void Entity::setRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	_rotation.setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);

	_transformNeedsUpdate = true;
}

void Entity::setRotation(const Matrix3& rot)
{
	_rotation = rot;

	_transformNeedsUpdate = true;
}

void Entity::addRotation(const Matrix3& rot)
{
	// Pre-multiply to apply rotation in local space
	rot.multiply(_rotation, &_rotation);

	_transformNeedsUpdate = true;
}

Vector3 Entity::getScaling() const
{
	return _scale;
}

void Entity::setScaling(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;

	_transformNeedsUpdate = true;
}

void Entity::addScaling(float x, float y, float z)
{
	_scale.x += x;
	_scale.y += y;
	_scale.z += z;

	_transformNeedsUpdate = true;
}

Matrix4 Entity::getMatrix()
{
	updateTransform();
	return _transform;
}

void Entity::updateTransform()
{
	if (!_transformNeedsUpdate)
	{
		return;
	}

	Matrix4::fromTranslation(_position, &_transform);
	_transform.multiply(Matrix4::fromRotation(_rotation), &_transform);
	_transform.multiply(Matrix4::fromScaling(_scale.x, _scale.y, _scale.z), &_transform);

	_transformNeedsUpdate = false;
}

Entity* Entity::getParent() const
{
	return _parent;
}

unsigned int Entity::getNumberOfChildren() const
{
	return _children.size();
}

void Entity::add(EntityPtr child)
{
	if (child->_parent != nullptr)
	{
		child->_parent->remove(child);
	}

	_children.push_back(child);
	child->_parent = this;
}

void Entity::remove(EntityPtr child)
{
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());

	if (child->_parent == this)
	{
		child->_parent = nullptr;
	}
}