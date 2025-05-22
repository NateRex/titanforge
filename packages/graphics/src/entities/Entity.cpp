#include <graphics/entities/Entity.h>
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
}

void Entity::addPosition(float x, float y, float z)
{
	_position.x += x;
	_position.y += y;
	_position.z += z;
}

Matrix3 Entity::getRotation() const
{
	return _rotation;
}

void Entity::setRotation(const Matrix3& rot)
{
	_rotation = rot;
}

void Entity::addRotation(const Matrix3& rot)
{
	_rotation.multiply(rot, &_rotation);
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
}

void Entity::addScaling(float x, float y, float z)
{
	_scale.x += x;
	_scale.y += y;
	_scale.z += z;
}

Matrix4 Entity::getMatrix() const
{
	Matrix4 m = Matrix4::fromTranslation(_position);
	m.multiply(Matrix4::fromRotation(_rotation), &m);
	m.multiply(Matrix4::fromScaling(_scale.x, _scale.y, _scale.z), &m);
	return m;
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