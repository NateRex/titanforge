#include <graphics/core/Entity.h>
#include <math/Matrix4.h>
#include <algorithm>

Entity::Entity(EntityType type): entityType(type), _scale(1.f, 1.f, 1.f)
{

}

Vector3 Entity::getPosition() const
{
	return _position;
}

void Entity::setPosition(float x, float y, float z)
{
	updatePosition(x, y, z);
}

void Entity::setPosition(const Vector3& v)
{
	updatePosition(v.x, v.y, v.z);
}

void Entity::addPosition(float x, float y, float z)
{
	updatePosition(_position.x + x, _position.y + y, _position.z + z);
}

void Entity::addPosition(const Vector3& v)
{
	updatePosition(_position.x + v.x, _position.y + v.y, _position.z + v.z);
}

Matrix3 Entity::getRotation() const
{
	return _rotation;
}

void Entity::setRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	updateRotation(m00, m01, m02, m10, m11, m12, m20, m21, m22);
}

void Entity::setRotation(const Matrix3& rot)
{
	updateRotation(rot[0], rot[1], rot[2], rot[3], rot[4], rot[5], rot[6], rot[7], rot[8]);
}

void Entity::addRotation(const Matrix3& rot)
{
	// Pre-multiply to apply rotation in local space
	Matrix3 r = rot.multiply(_rotation);
	
	updateRotation(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8]);
}

Vector3 Entity::getScaling() const
{
	return _scale;
}

void Entity::setScaling(float scalar)
{
	updateScaling(scalar, scalar, scalar);
}

void Entity::setScaling(float x, float y, float z)
{
	updateScaling(x, y, z);
}

void Entity::addScaling(float scalar)
{
	updateScaling(_scale.x + scalar, _scale.y + scalar, _scale.z + scalar);
}

void Entity::addScaling(float x, float y, float z)
{
	updateScaling(_scale.x + x, _scale.y + y, _scale.z + z);
}

Matrix4 Entity::getWorldMatrix()
{
	updateTransform();
	return _transform;
}

Matrix3 Entity::getNormalMatrix()
{
	updateTransform();
	Matrix4 matrix;
	if (!_transform.inverse(&matrix))
	{
		return Matrix3::IDENTITY;
	}

	matrix.transpose(&matrix);
	return Matrix3(matrix);
}

void Entity::updatePosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_transformNeedsUpdate = true;
}

void Entity::updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	_rotation.setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);
	_transformNeedsUpdate = true;
}

void Entity::updateScaling(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
	_transformNeedsUpdate = true;
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