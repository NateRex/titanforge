#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/windows/Window.h>
#include <graphics/primitives/PPolyface.h>
#include <graphics/primitives/PrimitiveAttributes.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>

/**
 * Helper method for constructing a sample polyface primitive for use in tests. This polyface
 * will contain 4 unique positions with 2 triangular facets referencing those positions.
 * @param includeColor True if the primitive should define colors for each vertex position. False otherwise.
 * @param includeTexCoords True if the primitive should define texture coordinates for each vertex position.
 * False otherwise.
 * @return The resulting polyface primitive
 */
PPolyface createExample(bool includeColor, bool includeTexCoords)
{
	const Vector3 verts[] = {
		Vector3(-1, -1, 0),
		Vector3(-1, 1, 0),
		Vector3(1, 1, 0),
		Vector3(1, -1, 0)
	};
	const int indices[] = {
		0, 1, 2, -1,
		0, 2, 3, -1
	};
	const Color colors[] = {
		Color::fromFloats(1, 0, 0, 1),
		Color::fromFloats(0, 1, 0, 1),
		Color::fromFloats(0, 0, 1, 1),
		Color::fromFloats(1, 1, 0, 1)
	};
	const Vector2 texCoords[] = {
		Vector2(0, 0),
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(1, 0)
	};

	return PPolyface(verts, 4, indices, 8,
		includeColor ? colors : nullptr,
		includeTexCoords ? texCoords : nullptr);
}

/**
 * Tests the basic constructors of the class
 */
BOOST_AUTO_TEST_CASE(PPolyface_basics)
{
	// Test using raw constructor
	PPolyface p = createExample(false, false);
	BOOST_TEST(p.getNumVertices() == 6);
	BOOST_TEST(p.getNumFacets() == 2);

	// Test using existing polyface
	PPolyface p2(p);
	BOOST_TEST(p2.getNumVertices() == p.getNumVertices());
	BOOST_TEST(p2.getNumFacets() == p.getNumFacets());
}

/**
 * Tests the ability to obtain the attributes of a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_attributes)
{
	// Without attributes
	PPolyface p = createExample(false, false);
	PrimitiveAttributes attributes = p.getAttributes();
	BOOST_TEST(!attributes.hasColor);
	BOOST_TEST(!attributes.hasTextureCoords);

	// With attributes
	p = createExample(true, true);
	attributes = p.getAttributes();
	BOOST_TEST(attributes.hasColor);
	BOOST_TEST(attributes.hasTextureCoords);
}

/**
 * Tests that buffer data can be obtained from a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_buffer)
{
	// Without attributes
	PPolyface p = createExample(false, false);
	std::vector<float> vBuffer;
	std::vector<int> iBuffer;
	p.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 12);
	BOOST_TEST(iBuffer.size() == 6);

	// With color
	p = createExample(true, false);
	vBuffer.clear();
	iBuffer.clear();
	p.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 28);
	BOOST_TEST(iBuffer.size() == 6);

	// With color and texture coordinates
	p = createExample(true, true);
	vBuffer.clear();
	iBuffer.clear();
	p.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 36);
	BOOST_TEST(iBuffer.size() == 6);
}