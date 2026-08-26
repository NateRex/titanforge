#include <boost/test/unit_test.hpp>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <graphics/core/buffers/GeometryBuffer.h>
#include <glad/glad.h>

/**
 * Tests conversion from engine primitive types to OpenGL primitive types
 */
BOOST_AUTO_TEST_CASE(Geometry_toGLPrimitive)
{
	BOOST_TEST(toGLPrimitive(PrimitiveType::POINTS) == GL_POINTS);
	BOOST_TEST(toGLPrimitive(PrimitiveType::TRIANGLES) == GL_TRIANGLES);
}

/**
 * Tests that a geometry is initially empty on construction
 */
BOOST_AUTO_TEST_CASE(Geometry_construction)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);
	BOOST_TEST(geom->size() == 0);

	GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.colors);
	BOOST_TEST(!attributes.uvs);
}

/**
 * Tests the ability to set the vertices and indices of a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_verticesAndIndices)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float vertices[] = {
		0.f, 0.f, 0.f,
		1.f, 2.f, 0.f,
		2.f, 0.f, 0.f 
	};

	// Set vertices, no indices
	geom->setVertices(vertices, 3, false);
	BOOST_TEST(geom->size() == 3);

	// Set vertices and indices together
	geom->setVertices(vertices, 3);
	BOOST_TEST(geom->size() == 3);

	// Update indices so that now there are 4 vertices in total
	unsigned int indices[] = { 0, 1, 2, 0 };
	geom->setIndices(indices, 4);
	BOOST_TEST(geom->size() == 4);
}

/**
 * Tests the ability to add and remove vertex normals from a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_normals)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float normals[] = {
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f
	};
	geom->setNormals(normals, 3);
	BOOST_TEST(geom->getAttributes().normals);

	geom->removeNormals();
	BOOST_TEST(!geom->getAttributes().normals);
}

/**
 * Tests the ability to add and remove colors from a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_colors)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

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
}

/**
 * Tests the ability to add and remove texture coordinates from a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_uvs)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float uvs[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 0.f };
	geom->setTextureCoords(uvs, 3);
	BOOST_TEST(geom->getAttributes().uvs);

	geom->removeTextureCoords();
	BOOST_TEST(!geom->getAttributes().uvs);
}

/**
 * Tests the ability to construct a GL buffer containing the data for a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_buffer)
{
	GeometryPtr geometry = Geometry::create(PrimitiveType::TRIANGLES);
	float vertices[] = {
		0.f, 0.f, 0.f,
		1.f, 2.f, 0.f,
		2.f, 0.f, 0.f
	};
	geometry->setVertices(vertices, 3);

	GeometryBuffer* buffer = geometry->getBuffer();
	BOOST_TEST(buffer != nullptr);
	BOOST_TEST(buffer->size() == geometry->size());
}