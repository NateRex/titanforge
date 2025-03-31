#include <graphics/entities/Entity.h>

Entity::Entity(): scale(1.f, 1.f, 1.f)
{

}

EntityPtr Entity::create()
{
	return std::shared_ptr<Entity>(new Entity());
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