#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector3.h>

/**
 * Tests the basic constructors of the class
 */
BOOST_AUTO_TEST_CASE(PPolyface_basics)
{
	// Test using raw arrays
	Vector3 pts[4] = {
		Vector3(-1, -1, 0),
		Vector3(1, -1, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};
	int verts[16] = {
		0, 1, 2, -1,
		0, 1, 3, -1,
		1, 2, 3, -1,
		2, 0, 3, -1
	};

	PPolyface p(pts, 4, verts, 16);
	BOOST_TEST(p.getNumVertices() == 12);
	BOOST_TEST(p.getNumFacets() == 4);

	// Test using vectors
	std::vector<Vector3> ptsVec(pts, pts + 4);
	std::vector<int> vertsVec(verts, verts + 16);
	p = PPolyface(ptsVec, vertsVec);
	BOOST_TEST(p.getNumVertices() == 12);
	BOOST_TEST(p.getNumFacets() == 4);

	// Test using existing polyface
	PPolyface p2(p);
	BOOST_TEST(p2.getNumVertices() == 12);
	BOOST_TEST(p2.getNumFacets() == 4);
}

/**
 * Tests that buffer data can be obtained from a polyface primitive
 */
BOOST_AUTO_TEST_CASE(PPolyface_buffer)
{
	Vector3 pts[4] = {
		Vector3(-1, -1, 0),
		Vector3(1, -1, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};
	int verts[16] = {
		0, 1, 2, -1,
		0, 1, 3, -1,
		1, 2, 3, -1,
		2, 0, 3, -1
	};
	PPolyface poly(pts, 4, verts, 16);

	std::vector<float> vertexBuffer;
	std::vector<int> indexBuffer;
	poly.buffer(vertexBuffer, indexBuffer);
	BOOST_TEST(vertexBuffer.size() == 12);
	BOOST_TEST(indexBuffer.size() == 12);
}