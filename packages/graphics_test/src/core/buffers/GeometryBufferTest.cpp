#include <boost/test/unit_test.hpp>
#include <graphics/core/buffers/GeometryBuffer.h>

/**
 * Tests the ability to create and bind a buffer using only vertex positions (no indices)
 */
BOOST_AUTO_TEST_CASE(GeometryBuffer_positionsOnly)
{
	float vertices[] = {
		-1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};

	GeometryBuffer buffer(GeometryAttributes(), vertices, 9, nullptr, 0);
	BOOST_TEST(buffer.size() == 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing vertex positions and indices
 */
BOOST_AUTO_TEST_CASE(GeometryBuffer_withIndices)
{
	unsigned int indices[] = { 0, 1, 2 };
	float vertices[] = {
		-1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};

	GeometryBuffer buffer(GeometryAttributes(), vertices, 9, indices, 3);
	BOOST_TEST(buffer.size() == 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing vertex positions, indices, and normals
 */
BOOST_AUTO_TEST_CASE(GeometryBuffer_withNormals)
{
	unsigned int indices[] = { 0, 1, 2 };
	float vertices[] = {
		1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 0.f, 0.f, 0.f, 1.f
	};
	GeometryAttributes attrib;
	attrib.normals = true;

	GeometryBuffer buffer(attrib, vertices, 18, indices, 3);
	BOOST_TEST(buffer.size() == 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing vertex positions, indices, and colors
 */
BOOST_AUTO_TEST_CASE(GeometryBuffer_withColors)
{
	unsigned int indices[] = { 0, 1, 2 };
	float vertices[] = {
		-1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f
	};
	GeometryAttributes attrib;
	attrib.colors = true;

	GeometryBuffer buffer(attrib, vertices, 21, indices, 3);
	BOOST_TEST(buffer.size() == 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}

/**
 * Tests the ability to create and bind a buffer using data containing vertex positions, indices, and and texture coordinates
 */
BOOST_AUTO_TEST_CASE(GeometryBuffer_withTextureCoords)
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

	GeometryBuffer buffer(attrib, vertices, 15, indices, 3);
	BOOST_TEST(buffer.size() == 3);
	BOOST_REQUIRE_NO_THROW(buffer.bind());
}