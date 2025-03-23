#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/windows/Window.h>
#include <graphics/primitives/TriangulatedPolyface.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <vector>

/**
 * Helper method for constructing a sample polyface primitive for use in tests. This polyface
 * will contain 4 unique positions with 2 triangular facets referencing those positions.
 * @param includeColor True if the primitive should define colors for each vertex position. False otherwise.
 * @param includeTexture True if the primitive should define texture coordinates for each vertex position.
 * False otherwise.
 * @return The resulting polyface primitive
 */
TriangulatedPolyface createExample(bool includeColor, bool includeTexture)
{
	std::vector<float> vertices;
	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(i);
		vertices.push_back(i * i + 1);
		vertices.push_back(0.f);

		if (includeColor)
		{
			vertices.push_back(1.f);
			vertices.push_back(0.f);
			vertices.push_back(0.f);
			vertices.push_back(0.f);
		}
		if (includeTexture)
		{
			vertices.push_back(i & 1);
			vertices.push_back(i >> 1);
		}
	}

	return TriangulatedPolyface(vertices.data(), vertices.size(), { includeColor, includeTexture });
}

/**
 * Tests the basic constructors of the class
 */
BOOST_AUTO_TEST_CASE(TriangulatedPolyface_basics)
{
	// Test using raw constructor
	TriangulatedPolyface p = createExample(false, false);
	BOOST_TEST(p.getSize() == 3);

	// Test using existing polyface
	TriangulatedPolyface p2(p);
	BOOST_TEST(p2.getSize() == 3);
}

/**
 * Tests the ability to obtain the attributes of a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_attributes)
{
	// Without attributes
	TriangulatedPolyface p = createExample(false, false);
	VertexAttributes attributes = p.getVertexAttributes();
	BOOST_TEST(!attributes.hasColor);
	BOOST_TEST(!attributes.hasTexture);

	// With attributes
	TriangulatedPolyface p2 = createExample(true, true);
	attributes = p2.getVertexAttributes();
	BOOST_TEST(attributes.hasColor);
	BOOST_TEST(attributes.hasTexture);
}

/**
 * Tests that buffer data can be obtained from a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_buffer)
{
	// Without attributes
	TriangulatedPolyface p = createExample(false, false);
	std::vector<float> vBuffer;
	std::vector<int> iBuffer;
	p.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 9);
	BOOST_TEST(iBuffer.size() == 3);

	// With color
	TriangulatedPolyface p2 = createExample(true, false);
	vBuffer.clear();
	iBuffer.clear();
	p2.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 21);
	BOOST_TEST(iBuffer.size() == 3);

	// With color and texture coordinates
	TriangulatedPolyface p3 = createExample(true, true);
	vBuffer.clear();
	iBuffer.clear();
	p3.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 27);
	BOOST_TEST(iBuffer.size() == 3);
}