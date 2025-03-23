#pragma once
#include <vector>

/**
 * Set of flags used to describe the data included with each vertex in a primitive.
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
	 * @return True if this set of primitive attributes is equal to the one given. Returns false otherwise.
	 */
	bool operator==(const VertexAttributes& other) const;

	/**
	 * Operator that compares this set of attributes to another and tests for inequality
	 * @param other The set of attributes to compare to
	 * @return True if this set of primitive attributes is not equal to the one given. Returns false otherwise.
	 */
	bool operator!=(const VertexAttributes& other) const;
};


/**
 * Abstract class for all primitives
 * @author Nathaniel Rex
 */
class Primitive
{

public:

	/**
	 * @return The set of attributes defining what data is included for each vertex on this primitive
	 */
	const VertexAttributes getVertexAttributes() const;

	/**
	 * Stores the data of this primitive in the given buffers
	 * @param vertexData Vector in which the data for each unique vertex should be stored.
	 * @param indices Vector to which the vertex indices of this primitive should be appended
	 */
	virtual void buffer(std::vector<float>& vertexData, std::vector<int>& indices) const = 0;

protected:

	/**
	 * The set of attributes defining what data is included for each vertex on this primitive
	 */
	const VertexAttributes _vertexAttributes;

	/**
	 * Constructor
	 * @param vertexAttributes The set of attributes defining what data is included for each
	 * vertex on this primitive
	 */
	Primitive(const VertexAttributes& vertexAttributes);
};