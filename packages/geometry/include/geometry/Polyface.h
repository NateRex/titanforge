#pragma once
#include <vector>
#include <iterator>
#include <cstddef>

class Vector3;

/**
 * A three-dimensional polyface
 * @author Nathaniel Rex
 */
class Polyface
{
public:

	/**
	 * Iterator used to step over the facets of a polyface.
	 * @author Nathaniel Rex
	 */
	class Iterator
	{
	public:	
		using iterator_category = std::input_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::vector<Vector3>;
		using pointer = int*;
		using reference = int&;

		/**
		 * Constructor
		 * @param verts Polyface positions
		 * @param indices Polyface indices
		 * @param itr Vertex iterator
		 */
		Iterator(const std::vector<Vector3>& verts, const std::vector<int>& indices,
			const std::vector<int>::const_iterator& itr);

		/**
		 * @return An iterator representing the next facet of the polyface
		 */
		Iterator& operator++();

		/**
		 * Determines whether this iterator and the given one are equal
		 * @param other Iterator to compare to
		 * @return True if the two iterators point to the same data. Returns false otherwise.
		 */
		bool operator==(const Iterator& other) const;

		/**
		 * Determines whether this iterator and the given one are not equal
		 * @param other Iterator to compare to
		 * @return True if the two iterators do not point to the same data. Returns false otherwise.
		 */
		bool operator!=(const Iterator& other) const;

		/**
		 * @return The points that make up the current facet
		 */
		std::vector<Vector3> operator*() const;

	private:

		/**
		 * Pointer to the vector of polyface positions
		 */
		const std::vector<Vector3>* _vertices;

		/**
		 * Pointer to the vector of polyface vertices
		 */
		const std::vector<int>* _indices;

		/**
		 * Vertex iterator
		 */
		std::vector<int>::const_iterator _itr;

		/**
		 * Helper method that moves the vertex iterator to the start of the next facet.
		 * If there is no next facet, the iterator will remain unchanged.
		 */
		void toNextFacet();
	};


public:

	/**
	 * Constructor
	 * @param verts Vertex positions
	 * @param numVerts Number of values in the position array
	 * @param indices Indices pointing to the ordered vertices of this polyface. A value of -1 implies
	 * the end of a facet and the start of another
	 * @param numIndices Number of values in the index array (including -1 values representing the ends
	 * of facets)
	 */
	Polyface(const Vector3* verts, int numVerts, const int* indices, int numIndices);

	/**
	 * Constructor
	 * @param vertices Vertex positions
	 * @param indices Indices pointing to the ordered vertices of this polyface. A value of -1 implies
	 * the end of a facet and the start of another
	 */
	Polyface(const std::vector<Vector3>& vertices, const std::vector<int> indices);

	/**
	 * Constructor
	 * @param polyface Polyface to copy data from
	 */
	Polyface(const Polyface& polyface);

	/**
	 * @return The total number of vertices in this polyface. Vertices of shared facets will
	 * be counted multiple times.
	 */
	int getNumVertices() const;

	/**
	 * @return The number of unique vertex positions in this polyface. Vertex positions shared by multiple
	 * facets will be counted once.
	 */
	int getNumPositions() const;

	/**
	 * @return The total number of facets in this polyface.
	 */
	int getNumFacets() const;

	/**
	 * @return An iterator pointing to the first facet of this polyface.
	 */
	Iterator begin() const;

	/**
	 * @return An iterator pointing to the theoretical element after the last facet of this polyface.
	 */
	Iterator end() const;
	
protected:

	/**
	 * Vertex positions
	 */
	std::vector<Vector3> _vertices;

	/**
	 * Indices pointing to the ordered vertices of this polyface. A value of -1 implies the end of a facet and the
	 * start of another
	 */
	std::vector<int> _indices;
};