#include <boost/test/unit_test.hpp>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>

/**
 * Tests the basic constructors, accessors, and mutators of the class
 */
BOOST_AUTO_TEST_CASE(Geometry_basics)
{
	GeometryPtr geom = Geometry::create();
	BOOST_TEST(geom->size() == 0);
	BOOST_TEST(!geom->getAttributes().colors);
	BOOST_TEST(!geom->getAttributes().uvs);

	// vertices
	float vertices[] = {
		0.f, 0.f, 0.f,
		1.f, 2.f, 0.f,
		2.f, 0.f, 0.f 
	};
	geom->setVertices(vertices, 3);
	BOOST_TEST(geom->size() == 3);

	// indices
	unsigned int indices[] = { 0, 1, 2, 0 };
	geom->setIndices(indices, 4);
	BOOST_TEST(geom->size() == 4);

	// colors
	float colors[] = {
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 0.f
	};
	geom->setColors(colors, 4);
	BOOST_TEST(geom->getAttributes().colors);
	geom->removeColors();
	BOOST_TEST(!geom->getAttributes().colors);

	// texture coords
	float uvs[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 0.f };
	geom->setTextureCoords(uvs, 3);
	BOOST_TEST(geom->getAttributes().uvs);
	geom->removeTextureCoords();
	BOOST_TEST(!geom.);
}