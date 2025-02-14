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
		using pointer = unsigned int*;
		using reference = unsigned int&;

		/**
		 * Constructor
		 */
		Iterator(const std::vector<Vector3>& positions, const std::vector<unsigned int>& vertices);

		/**
		 * @return An iterator representing the next facet of the polyface
		 */
		Iterator operator++();

		/**
		 * Determines whether this iterator and the given one are equal
		 * @param other Iterator to compare to
		 * @return True if the two iterators point to the same data. Returns false otherwise.
		 */
		bool operator==() const;

		/**
		 * Determines whether this iterator and the given one are not equal
		 * @param other Iterator to compare to
		 * @return True if the two iterators do not point to the same data. Returns false otherwise.
		 */
		bool operator!=() const;

		/**
		 * @return The points that make up the current facet
		 */
		std::vector<Vector3> operator*() const;

	private:

		/**
		 * Pointer to the vector of polyface positions
		 */
		const std::vector<Vector3>* _pos;

		/**
		 * Vertex iterator
		 */
		typename std::vector<unsigned int>::iterator _itr;
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