#pragma once
#include <vector>

class Vector3;

/**
 * A three-dimensional polyface
 * @author Nathaniel Rex
 */
class Polyface
{



private:

	/**
	 * Vertex positions
	 */
	std::vector<Vector3> _positions;

	/**
	 * Ordered vertices that make up the facets of this polyface. Each value is an index into
	 * the list of positions. A value of -1 implies the end of one facet and the beginning of
	 * another.
	 */
	std::vector<int> _vertices;
};