#include <boost/test/unit_test.hpp>
#include <graphics/primitives/Primitive.h>
#include <common/Utils.h>

// ------------------------------------------------------------------------------------------------------------------
// VertexAttributes
// ------------------------------------------------------------------------------------------------------------------

/**
 * Tests the basic construction of vertex attributes
 */
BOOST_AUTO_TEST_CASE(VertexAttributes_basics)
{
	VertexAttributes attrib;
	BOOST_TEST(!attrib.hasColor);
	BOOST_TEST(!attrib.hasTexture);

	attrib = { true, true };
	BOOST_TEST(attrib.hasColor);
	BOOST_TEST(attrib.hasTexture);
}

/**
 * Tests equality operators
 */
BOOST_AUTO_TEST_CASE(VertexAttributes_equalsChecks)
{
	VertexAttributes a1 = { false, false };
	BOOST_TEST(a1 == a1);

	VertexAttributes a2 = { true, false };
	BOOST_TEST(a1 != a2);

	a2 = { false, true };
	BOOST_TEST(a1 != a2);
}

/**
 * Tests the ability to get the stride of vertices, given the attributes included with each
 * vertex
 */
BOOST_AUTO_TEST_CASE(VertexAttributes_stride)
{
	VertexAttributes a = { false, false };
	BOOST_TEST(a.getStride() == 3);

	a = { true, false };
	BOOST_TEST(a.getStride() == 7);

	a = { false, true };
	BOOST_TEST(a.getStride() == 5);

	a = { true, true };
	BOOST_TEST(a.getStride() == 9);
}


// ------------------------------------------------------------------------------------------------------------------
// Primitive
// ------------------------------------------------------------------------------------------------------------------


/**
 * Helper method for constructing a sample primitive for use in tests. This triangulated polyface
 * will contain 1 facet with 3 unique positions.
 * @param includeColor True if the primitive should define colors for each vertex position. False otherwise.
 * @param includeTexture True if the primitive should define texture coordinates for each vertex position.
 * False otherwise.
 * @return The resulting polyface primitive
 */
Primitive createExample(bool includeColor, bool includeTexture)
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

	return Primitive(vertices.data(), vertices.size(), { includeColor, includeTexture });
}

/**
 * Tests the basic constructors of the class
 */
BOOST_AUTO_TEST_CASE(Primitive_basics)
{
	// Test using raw constructor
	Primitive p = createExample(false, false);
	BOOST_TEST(p.getSize() == 3);

	// Test using existing polyface
	Primitive p2(p);
	BOOST_TEST(p2.getSize() == 3);
}

/**
 * Tests the ability to obtain the attributes of a primitive
 */
BOOST_AUTO_TEST_CASE(Primitive_attributes)
{
	// Without attributes
	Primitive p = createExample(false, false);
	VertexAttributes attributes = p.getVertexAttributes();
	BOOST_TEST(!attributes.hasColor);
	BOOST_TEST(!attributes.hasTexture);

	// With attributes
	Primitive p2 = createExample(true, true);
	attributes = p2.getVertexAttributes();
	BOOST_TEST(attributes.hasColor);
	BOOST_TEST(attributes.hasTexture);
}

/**
 * Tests that buffer data can be obtained from a primitive
 */
BOOST_AUTO_TEST_CASE(Primitive_buffer)
{
	// Without attributes
	Primitive p = createExample(false, false);
	std::vector<float> vBuffer;
	std::vector<int> iBuffer;
	p.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 9);
	BOOST_TEST(iBuffer.size() == 3);

	// With color
	Primitive p2 = createExample(true, false);
	vBuffer.clear();
	iBuffer.clear();
	p2.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 21);
	BOOST_TEST(iBuffer.size() == 3);

	// With color and texture coordinates
	Primitive p3 = createExample(true, true);
	vBuffer.clear();
	iBuffer.clear();
	p3.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 27);
	BOOST_TEST(iBuffer.size() == 3);
}