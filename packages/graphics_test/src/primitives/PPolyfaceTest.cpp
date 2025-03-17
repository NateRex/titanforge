#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/windows/Window.h>
#include <graphics/primitives/PPolyface.h>
#include <graphics/primitives/PrimitiveAttributes.h>
#include <geometry/Vector3.h>

/**
 * Helper method for getting sample data that can be used to construct a PPolyface
 * @param vertices Vector in which to store vertex positions
 * @param indices Vector in which to store vertex indices
 * @param colors (Optional) Vector in which to store colors
 */
void getSampleData(std::vector<Vector3>& vertices, std::vector<int>& indices,
		std::vector<Color>& colors)
{
	vertices.push_back(Vector3(-1, -1, 0));
	vertices.push_back(Vector3(-1, 1, 0));
	vertices.push_back(Vector3(1, 1, 0));
	vertices.push_back(Vector3(1, -1, 0));

	colors.push_back(Color::fromFloats(1, 0, 0, 1));
	colors.push_back(Color::fromFloats(0, 1, 0, 1));
	colors.push_back(Color::fromFloats(0, 0, 1, 1));
	colors.push_back(Color::fromFloats(1, 1, 0, 1));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(-1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(-1);
}

/**
 * Tests the basic constructors of the class
 */
BOOST_AUTO_TEST_CASE(PPolyface_basics)
{
	std::vector<Vector3> vertices;
	std::vector<int> indices;
	std::vector<Color> colors;
	getSampleData(vertices, indices, colors);

	// Test using raw pointers
	PPolyface p(vertices.data(), vertices.size(), indices.data(), indices.size());
	BOOST_TEST(p.getNumVertices() == 6);
	BOOST_TEST(p.getNumFacets() == 2);

	// Test using vectors
	p = PPolyface(vertices.data(), vertices.size(), indices.data(), indices.size());
	BOOST_TEST(p.getNumVertices() == 6);
	BOOST_TEST(p.getNumFacets() == 2);

	// Test using existing polyface
	PPolyface p2(p);
	BOOST_TEST(p2.getNumVertices() == 6);
	BOOST_TEST(p2.getNumFacets() == 2);
}

/**
 * Tests the ability to obtain the attributes of a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_attributes)
{
	std::vector<Vector3> vertices;
	std::vector<Color> colors;
	std::vector<int> indices;
	getSampleData(vertices, indices, colors);

	// Without color
	PPolyface poly(vertices.data(), vertices.size(), indices.data(), indices.size());
	PrimitiveAttributes attributes = poly.getAttributes();
	BOOST_TEST(!attributes.hasColor);

	// With color
	poly = PPolyface(vertices, indices, colors);
	attributes = poly.getAttributes();
	BOOST_TEST(attributes.hasColor);
}

/**
 * Tests that buffer data can be obtained from a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_buffer)
{
	std::vector<Vector3> vertices;
	std::vector<Color> colors;
	std::vector<int> indices;
	getSampleData(vertices, indices, colors);

	// Without color
	PPolyface poly(vertices.data(), vertices.size(), indices.data(), indices.size());
	std::vector<float> vBuffer;
	std::vector<int> iBuffer;
	poly.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 12);
	BOOST_TEST(iBuffer.size() == 6);

	// With color
	poly = PPolyface(vertices, indices, colors);
	vBuffer.clear();
	iBuffer.clear();
	poly.buffer(vBuffer, iBuffer);
	BOOST_TEST(vBuffer.size() == 28);
	BOOST_TEST(iBuffer.size() == 6);
}