#pragma once
#include <vector>

/**
 * Set of flags used to describe the data included with each vertex in a mesh.
 * @author Nathaniel Rex
 */
struct VertexAttributes
{
	/**
	 * Flag that, when true, indicates that each vertex has color. Defaults to false.
	 */
	bool hasColor = false;

	/**
	 * Flag that, when true, indicates that each vertex has texture coordinates. Defaults to false.
	 */
	bool hasTexture = false;

	/**
	 * @return The stride representing the number of float values included in a single vertex, taking into account
	 * all attributes.
	 */
	int getStride() const;

	/**
	 * Operator that compares this set of attributes to another and tests for equality
	 * @param other The set of attributes to compare to
	 * @return True if this set of vertex attributes are equal to the one given. Returns false otherwise.
	 */
	bool operator==(const VertexAttributes& other) const;

	/**
	 * Operator that compares this set of attributes to another and tests for inequality
	 * @param other The set of attributes to compare to
	 * @return True if this set of vertex attributes are not equal to the one given. Returns false otherwise.
	 */
	bool operator!=(const VertexAttributes& other) const;
};


/**
 * A mesh in the form of a triangulated polyface, containing both geometry and attributes used for rendering.
 * @author Nathaniel Rex
 */
class Mesh
{
public:

	friend class BufferManager;

	/**
	 * Constructs a mesh from an array of vertices representing a triangulated polyface.
	 * @param vertices Vertex array. Each vertex is represented using a minimum of 3 values. The inclusion of other vertex
	 * attributes implies a larger number of values per vertex.
	 * @param verticesSize The number of values in the array
	 * @param vertexAttributes The attributes included with each vertex
	 * Defaults to false.
	 */
	Mesh(const float* vertices, int verticesSize, const VertexAttributes& vertexAttributes);

	/**
	 * Constructs a mesh from an array of vertices and an array of indices that together represent a triangulated polyface.
	 * @param vertices Vertex array. Each vertex is represented using a minimum of 3 values. The inclusion of other vertex
	 * attributes implies a larger number of values per vertex.
	 * @param verticesSize The number of values in the vertex array
	 * @param indicesSize Index array defining the ordering of vertices. The values in the index array refer to individual vertices,
	 * not accounting for stride. For example, if the vertex array contains six values representing two points
	 * (each with three components), valid index values are 0 or 1.
	 * @param numIndices The number of values in the index array
	 * @param vertexAttributes The attributes included with each vertex
	 */
	Mesh(const float* vertices, int verticesSize, const int* indices, int indicesSize,
		const VertexAttributes& vertexAttributes);

	/**
	 * Constructs a mesh from an existing object
	 * @param Mesh Existing mesh
	 */
	Mesh(const Mesh& mesh);

	/**
	 * Destructor
	 */
	~Mesh();

	/**
	 * @return The total number of vertices across all facets of this mesh
	 */
	int getSize() const;

	/**
	 * @return The set of attributes defining what data is included for each vertex on this mesh
	 */
	const VertexAttributes getVertexAttributes() const;

	/**
	 * Stores the data of this mesh in the given buffers
	 * @param vertexData Vector in which the data for each unique vertex should be stored.
	 * @param indices Vector to which the vertex indices of this mesh should be appended
	 */
	void buffer(std::vector<float>& vertexData, std::vector<int>& indices) const;

protected:

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

	/**
	 * The set of attributes defining what data is included for each vertex on this mesh
	 */
	const VertexAttributes _vertexAttributes;
};