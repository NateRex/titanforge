#pragma once
#include <graphics/primitives/Primitive.h>

/**
 * A three-dimensional triangulated polyface primitive capable of being rendered
 * @author Nathaniel Rex
 */
class TriangulatedPolyface: public Primitive
{
public:

	/**
	 * Constructs a polyface primitive from an array of vertices representing a triangulated polyface.
	 * @param vertices Vertex array. Each vertex is represented using a minimum of 3 values. The inclusion of other vertex
	 * attributes implies a larger number of values per vertex.
	 * @param verticesSize The number of values in the array
	 * @param vertexAttributes The attributes included with each vertex
	 * Defaults to false.
	 */
	TriangulatedPolyface(const float* vertices, int verticesSize, const VertexAttributes& vertexAttributes);

	/**
	 * Constructs a polyface primitive from an array of vertices and an array of indices that together represent a triangulated
	 * polyface.
	 * @param vertices Vertex array. Each vertex is represented using a minimum of 3 values. The inclusion of other vertex
	 * attributes implies a larger number of values per vertex.
	 * @param verticesSize The number of values in the vertex array
	 * @param indicesSize Index array defining the ordering of vertices. The values in the index array refer to individual vertices,
	 * not accounting for stride. For example, if the vertex array contains six values representing two points
	 * (each with three components), valid index values are 0 or 1.
	 * @param numIndices The number of values in the index array
	 * @param vertexAttributes The attributes included with each vertex
	 */
	TriangulatedPolyface(const float* vertices, int verticesSize, const int* indices, int indicesSize,
		const VertexAttributes& vertexAttributes);

	/**
	 * Constructs a triangulated polyface primitive from an existing object
	 * @param polyface Existing polyface
	 */
	TriangulatedPolyface(const TriangulatedPolyface& polyface);

	/**
	 * Destructor
	 */
	~TriangulatedPolyface();

	/**
	 * @return The total number of vertices across all facets of this polyface
	 */
	int getSize() const;

	/**
	 * Stores the data of this primitive in the given buffers
	 * @param vertexData Vector in which the data for each unique vertex should be stored.
	 * @param indices Vector to which the vertex indices of this primitive should be appended
	 */
	void buffer(std::vector<float>& vertexData, std::vector<int>& indices) const;

private:

	/**
	 * Vertex data. Each vertex is represented using a minimum of 3 values. The inclusion of other vertex
	 * attributes implies a larger number of values per vertex.
	 */
	float* _vertices;

	/**
	 * The number of values in the vertex array
	 */
	int _verticesSize;

	/**
	 * Index array defining the ordering of vertices. The values in the index array refer to individual vertices,
	 * not accounting for stride. For example, if the vertex array contains six values representing two points
	 * (each with three components), valid index values are 0 or 1.
	 */
	int* _indices;

	/**
	 * The number of values in the index array
	 */
	int _indicesSize;
};