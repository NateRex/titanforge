#include <boost/test/unit_test.hpp>
#include <geometry/Polyface.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>

/**
 * Basic tests for constructors and accessors of the polyface class
 */
BOOST_AUTO_TEST_CASE(Polyface_basics)
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

	Polyface p(pts, 4, verts, 16);
	BOOST_TEST(p.getNumPositions() == 4);
	BOOST_TEST(p.getNumVertices() == 12);
	BOOST_TEST(p.getNumFacets() == 4);

	// Test using vectors
	std::vector<Vector3> ptsVec(pts, pts + 4);
	std::vector<int> vertsVec(verts, verts + 16);
	p = Polyface(ptsVec, vertsVec);
	BOOST_TEST(p.getNumPositions() == 4);
	BOOST_TEST(p.getNumVertices() == 12);
	BOOST_TEST(p.getNumFacets() == 4);

	// Test using existing polyface
	Polyface p2(p);
	BOOST_TEST(p2.getNumPositions() == 4);
	BOOST_TEST(p2.getNumVertices() == 12);
	BOOST_TEST(p2.getNumFacets() == 4);
}

/**
 * Tests that extraneous facet-end markers have no effect, and that a closing facet-marker at the end of
 * the vertex array is optional
 */
BOOST_AUTO_TEST_CASE(Polyface_extraneousMarkers)
{
	Vector3 pts[4] = {
		Vector3(-1, -1, 0),
		Vector3(1, -1, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};
	int verts[20] = {
		-1, -1,
		0, 1, 2, -1,
		0, 1, 3, -1, -1,
		1, 2, 3, -1, -1, -1,
		2, 0, 3
	};

	Polyface p(pts, 4, verts, 20);
	BOOST_TEST(p.getNumPositions() == 4);
	BOOST_TEST(p.getNumVertices() == 12);
	BOOST_TEST(p.getNumFacets() == 4);
}

/**
 * Tests the ability to iterate over the facets of a polyface
 */
BOOST_AUTO_TEST_CASE(Polyface_iteration)
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
	Vector3 expected[][3] = {
		{ pts[0], pts[1], pts[2] },
		{ pts[0], pts[1], pts[3] },
		{ pts[1], pts[2], pts[3] },
		{ pts[2], pts[0], pts[3] }
	};

	Polyface p(pts, 4, verts, 16);

	int fIdx = 0;
	for (Polyface::Iterator itr = p.begin(); itr != p.end(); ++itr)
	{
		std::vector<Vector3> facet = *itr;
		BOOST_TEST(facet.size() == 3);

		for (int i = 0; i < 3; i++)
		{
			BOOST_TEST(facet.at(i) == expected[fIdx][i]);
		}

		fIdx++;
	}
}