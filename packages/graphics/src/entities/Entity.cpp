#include <graphics/entities/Entity.h>
#include <math/Matrix4.h>
#include <algorithm>

Entity::Entity(EntityType type): type(type), scale(1.f, 1.f, 1.f)
{

}

void Entity::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Entity::addPosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Entity::setRotation(const Matrix3& rot)
{
	rotation = rot;
}

void Entity::addRotation(const Matrix3& rot)
{
	rotation.multiply(rot, &rotation);
}

void Entity::setScaling(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Entity::addScaling(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
}

Matrix4 Entity::getWorldMatrix() const
{
	std::vector<const Entity*> parentList;

	// Traverse up parent chain
	const Entity* current = this;
	while (current != nullptr)
	{
		parentList.push_back(current);
		current = current->getParent();
	}

	// Reverse order
	std::reverse(parentList.begin(), parentList.end());

	// Apply matrices
	Matrix4 result;
	for (const Entity* e : parentList)
	{
		result.multiply(e->getLocalToParentMatrix(), &result);
	}

	return result;
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

Matrix4 Entity::getLocalToParentMatrix() const
{
	Matrix4 m = Matrix4::fromTranslation(position);
	m.multiply(Matrix4::fromRotation(rotation), &m);
	m.multiply(Matrix4::fromScaling(scale.x, scale.y, scale.z), &m);
	return m;
}