#include <boost/test/unit_test.hpp>
#include <graphics/entities/Entity.h>
#include <common/Utils.h>

/**
 * Extension of the Entity class used for testing
 * @author Nathaniel Rex
 */
class TestEntity : public Entity
{
public:

	/**
	 * Constructor
	 */
	TestEntity(): Entity(EntityType::GROUP)
	{

	}
};

/**
 * Tests the basic construction and accessors of the class
 */
BOOST_AUTO_TEST_CASE(Entity_basics)
{
	TestEntity e;
	BOOST_TEST(e.position == Vector3());
	BOOST_TEST(e.rotation == Matrix3::IDENTITY);
	BOOST_TEST(e.scale == Vector3(1.f, 1.f, 1.f));
}

/**
 * Tests setting and adding position using float values
 */
BOOST_AUTO_TEST_CASE(Entity_positionFromFloats)
{
	TestEntity e;
	
	e.setPosition(1, 2, 3);
	BOOST_TEST(e.position == Vector3(1, 2, 3));

	e.addPosition(1, 2, 3);
	BOOST_TEST(e.position == Vector3(2, 4, 6));
}

/**
 * Tests setting and adding rotation using a matrix
 */
BOOST_AUTO_TEST_CASE(Entity_rotationFromMatrix)
{
	TestEntity e;

	Matrix3 rot30 = Matrix3::fromXRotation(deg2Rad(30.f));
	e.setRotation(rot30);
	BOOST_TEST(e.rotation == rot30);

	Matrix3 rot60 = rot30.multiply(rot30);
	e.addRotation(rot30);
	BOOST_TEST(e.rotation == rot60);
}

/**
 * Tests setting and adding scale using float values
 */
BOOST_AUTO_TEST_CASE(Entity_scaleFromFloats)
{
	TestEntity e;

	e.setScaling(1, 2, 3);
	BOOST_TEST(e.scale == Vector3(1, 2, 3));
	
	e.addScaling(1, 2, 3);
	BOOST_TEST(e.scale == Vector3(2, 4, 6));
}