#pragma once
#include <vector>

class Vector3;

/**
 * A three-dimensional polyface
 * @author Nathaniel Rex
 */
class Polyface
{
public:

	/**
	 * Iterator used to step over the facets of a polyface
	 * @author Nathaniel Rex
	 */
	class FacetIterator
	{
	public:
		
		/**
		 * @return True if the polyface has a next facet. Returns false otherwise.
		 */
		bool hasNextFacet() const;
	
		/**
		 * Moves this iterator to the next facet of the polyface
		 */
		void operator++();

		/**
		 * @return The ordered vector of points for the current facet
		 */
		std::vector<Vector3> operator*() const;

	private:
		
		/**
		 * Vertex
		 */

		/**
		 * The index representing the last vertex that was parsed from the polyface
		 */
		unsigned int _previous = 0;
	};


public:

	/**
	 * Constructor
	 * @param positions Vertex positions
	 * @param vertices Ordered vertices that make up the facets of this polyface. Each value
	 * should represents an index into the list of positions. A value of -1 implies the end
	 * of a facet and the start of another
	 */
	Polyface(const std::vector<Vector3>& positions, const std::vector<unsigned int> vertices);

	/**
	 * @return The total number of vertices in this polyface. Vertices of shared facets will
	 * be counted multiple times.
	 */
	unsigned int getNumVertices() const;

	
private:

	/**
	 * Vertex positions
	 */
	std::vector<Vector3> _positions;

	/**
	 * Ordered vertices that make up the facets of this polyface. Each value should represents
	 * an index into the list of positions. A value of -1 implies the end of a facet and the
	 * start of another
	 */
	std::vector<unsigned int> _vertices;
};