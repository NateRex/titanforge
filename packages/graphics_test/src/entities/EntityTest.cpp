#include <boost/test/unit_test.hpp>
#include <graphics/entities/Entity.h>
#include <common/Utils.h>

/**
 * Tests the basic construction and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Entity_basics)
{
	EntityPtr e = Entity::create();
	BOOST_TEST(e->position == Vector3());
	BOOST_TEST(e->rotation == Matrix3::IDENTITY);
	BOOST_TEST(e->scale == Vector3(1.f, 1.f, 1.f));
}

/**
 * Tests setting and adding position using float values
 */
BOOST_AUTO_TEST_CASE(Entity_positionFromFloats)
{
	EntityPtr e = Entity::create();
	
	e->setPosition(1, 2, 3);
	BOOST_TEST(e->position == Vector3(1, 2, 3));

	e->addPosition(1, 2, 3);
	BOOST_TEST(e->position == Vector3(2, 4, 6));
}

/**
 * Tests setting and adding rotation using a matrix
 */
BOOST_AUTO_TEST_CASE(Entity_rotationFromMatrix)
{
	EntityPtr e = Entity::create();

	Matrix3 rot30 = Matrix3::fromXRotation(deg2Rad(30.f));
	e->setRotation(rot30);
	BOOST_TEST(e->rotation == rot30);

	Matrix3 rot60 = rot30.multiply(rot30);
	e->addRotation(rot30);
	BOOST_TEST(e->rotation == rot60);
}

/**
 * Tests setting and adding scale using float values
 */
BOOST_AUTO_TEST_CASE(Entity_scaleFromFloats)
{
	EntityPtr e = Entity::create();

	e->setScaling(1, 2, 3);
	BOOST_TEST(e->scale == Vector3(1, 2, 3));
	
	e->addScaling(1, 2, 3);
	BOOST_TEST(e->scale == Vector3(2, 4, 6));
}