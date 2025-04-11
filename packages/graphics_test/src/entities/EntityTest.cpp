#include <boost/test/unit_test.hpp>
#include <graphics/entities/Entity.h>
#include <math/Matrix4.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>

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
	TestEntity(): Entity(EntityType::GROUP) { }
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

/**
 * Tests the ability to obtain the transformation matrix for an entity
 */
BOOST_AUTO_TEST_CASE(Entity_getTransform)
{
	TestEntity e;

	Matrix4 translation = Matrix4::fromTranslation(Vector3(1.f, 2.f, 3.f));
	e.setPosition(1.f, 2.f, 3.f);

	Matrix3 rot3x3 = Matrix3::fromXRotation(deg2Rad(45.f));
	Matrix4 rotation = Matrix4::fromRotation(rot3x3);
	e.setRotation(rot3x3);

	Matrix4 scale = Matrix4::fromScaling(4.f, 5.f, 6.f);
	e.setScaling(4.f, 5.f, 6.f);

	Matrix4 expected = translation.multiply(rotation).multiply(scale);
	Matrix4 transform = e.getMatrix();
	BOOST_TEST(transform.equalTo(expected));
}

/**
 * Tests the ability to add and remove children from an entity
 */
BOOST_AUTO_TEST_CASE(Entity_children)
{
	EntityPtr child = std::make_shared<TestEntity>();
	BOOST_TEST(child->getParent() == nullptr);

	TestEntity parent1;
	parent1.add(child);
	BOOST_TEST(child->getParent() == &parent1);
	BOOST_TEST(parent1.getNumberOfChildren() == 1);

	TestEntity parent2;
	parent2.add(child);
	BOOST_TEST(child->getParent() == &parent2);
	BOOST_TEST(parent1.getNumberOfChildren() == 0);
	BOOST_TEST(parent2.getNumberOfChildren() == 1);
}