#include <graphics/entities/Entity.h>
#include <math/Matrix4.h>

Entity::Entity(EntityType type): _type(type), scale(1.f, 1.f, 1.f)
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

Matrix4 Entity::getMatrix() const
{
	Matrix4 m = Matrix4::fromTranslation(position);
	m.multiply(Matrix4::fromRotation(rotation), &m);
	m.multiply(Matrix4::fromScaling(scale.x, scale.y, scale.z), &m);
	return m;
}