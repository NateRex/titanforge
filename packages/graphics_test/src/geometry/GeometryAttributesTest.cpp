#include <boost/test/unit_test.hpp>
#include <graphics/geometry/GeometryAttributes.h>
#include <common/PrintHelpers.h>
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
 * Tests the basic constructors and accessors
 */
BOOST_AUTO_TEST_CASE(GeometryAttributes_basics)
{
	GeometryAttributes attrib;
	BOOST_TEST(attrib.primitiveType == PrimitiveType::POINTS);
	BOOST_TEST(!attrib.indices);
	BOOST_TEST(!attrib.normals);
	BOOST_TEST(!attrib.colors);
	BOOST_TEST(!attrib.uvs);

	attrib = { PrimitiveType::TRIANGLES, true, true, true, true };
	BOOST_TEST(attrib.primitiveType == PrimitiveType::TRIANGLES);
	BOOST_TEST(attrib.indices);
	BOOST_TEST(attrib.normals);
	BOOST_TEST(attrib.colors);
	BOOST_TEST(attrib.uvs);
}

/**
 * Tests equality operators
 */
BOOST_AUTO_TEST_CASE(GeometryAttributes_equalsChecks)
{
	GeometryAttributes a1 = { PrimitiveType::TRIANGLES, false, false, false, false };
	BOOST_TEST(a1 == a1);

	GeometryAttributes a2 = { PrimitiveType::POINTS, false, false, false, false };
	BOOST_TEST(a1 != a2);

	a2 = { PrimitiveType::TRIANGLES, true, false, false, false };
	BOOST_TEST(a1 != a2);

	a2 = { PrimitiveType::TRIANGLES, false, true, false, false };
	BOOST_TEST(a1 != a2);

	a2 = { PrimitiveType::TRIANGLES, false, false, true, false };
	BOOST_TEST(a1 != a2);

	a2 = { PrimitiveType::TRIANGLES, false, false, false, true };
	BOOST_TEST(a1 != a2);
}

/**
 * Tests the ability to get the stride of vertices, given the attributes included with each
 * vertex
 */
BOOST_AUTO_TEST_CASE(GeometryAttributes_stride)
{
	GeometryAttributes a = { PrimitiveType::TRIANGLES, false, false, false, false };
	BOOST_TEST(a.getStride() == 3);

	// Normals
	a = { PrimitiveType::TRIANGLES, false, true, false, false };
	BOOST_TEST(a.getStride() == 6);

	// Colors
	a = { PrimitiveType::TRIANGLES, false, false, true, false };
	BOOST_TEST(a.getStride() == 7);

	// Texture coordinates
	a = { PrimitiveType::TRIANGLES, false, false, false, true };
	BOOST_TEST(a.getStride() == 5);

	// Ensure indices have no impact on stride
	a = { PrimitiveType::TRIANGLES, true, false, false, false };
	BOOST_TEST(a.getStride() == 3);
}