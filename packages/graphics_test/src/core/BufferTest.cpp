#include <boost/test/unit_test.hpp>
#include <graphics/core/Buffer.h>
#include <graphics/geometry/GeometryAttributes.h>

/**
 * Tests the ability to create and bind a buffer using only vertex positions
 */
BOOST_AUTO_TEST_CASE(Buffer_positionsOnly)
{
	float vertices[] = {
		-1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};
	unsigned int indices[] = { 0, 1, 2 };
	GeometryAttributes attrib;

	Buffer buffer(attrib, vertices, 9, indices, 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing vertex normals
 */
BOOST_AUTO_TEST_CASE(Buffer_withNormals)
{
	unsigned int indices[] = { 0, 1, 2 };
	float vertices[] = {
		1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 0.f, 0.f, 1.f
	};
	GeometryAttributes attrib;
	attrib.normals = true;

	Buffer buffer(attrib, vertices, 18, indices, 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing vertex colors
 */
BOOST_AUTO_TEST_CASE(Buffer_withColors)
{
	unsigned int indices[] = { 0, 1, 2 };
	float vertices[] = {
		-1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f
	};
	GeometryAttributes attrib;
	attrib.colors = true;

	Buffer buffer(attrib, vertices, 21, indices, 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing texture (UV) coordinates
 */
BOOST_AUTO_TEST_CASE(Buffer_withTextureCoords)
{
	unsigned int indices[] = { 0, 1, 2 };
	float vertices[] = {
		-1.f, 0.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.5f, 1.f,
		1.f, 0.f, 0.f, 1.f, 0.f
	};
	GeometryAttributes attrib;
	attrib.colors = false;
	attrib.uvs = true;

	Buffer buffer(attrib, vertices, 15, indices, 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}