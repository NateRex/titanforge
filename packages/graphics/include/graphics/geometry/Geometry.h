#pragma once
#include <vector>
#include <graphics/geometry/pointers/GeometryPtr.h>
#include <graphics/geometry/GeometryAttributes.h>

class Vector2;
class Vector3;
class Color;
class GeometryBuffer;
class InstanceBuffer;

/**
 * Base class for all geometry, which defines an object, line, or point in local space. Contains vertex attributes
 * such as position, color, texture coordinates, and unit normals.
 * @author Nathaniel Rex
 */
class Geometry
{
public:

	/**
	 * The geometric primitive type used to interpret the vertex data of this geometry
	 */
	const PrimitiveType type;

	/**
	 * Destructor
	 */
	~Geometry();

	/**
	 * Constructs a new geometry instance
	 * @param primitiveType The geometric primitive type that should be used to interpret the vertex data of this geometry
	 * @return The new geometry instance
	 */
	static GeometryPtr create(PrimitiveType primitiveType);

	/**
	 * Sets the vertex positions of this geometry. If the indices of this geometry have not yet been set,
	 * they will additionally be computed and set automatically.
	 * @param vertices An array where every 3 values represents the x, y, and z components of a vertex.
	 * @param numVertices The number of vertices in the array
	 * @param updateIndices Boolean flag that, when true, will cause the indices of this geometry to additionally be computed and
	 * set. This will only occur if they have not already been set explicitly via Geometry::setIndices. Defaults to true.
	 */
	void setVertices(const float* vertices, unsigned int numVertices, bool updateIndices = true);

	/**
	 * Sets the vertex indices of this geometry.
	 * @param indices An array whose values represent the index order of vertex positions, normals, and texture coordinates.
	 * @param numIndices The number of indices in the array
	 */
	void setIndices(const unsigned int* indices, unsigned int numIndices);

	/**
	 * @return The total number of points in this geometry. If indices are set, this will be the number of indices. Otherwise, it defaults
	 * to the number of vertices.
	 */
	unsigned int size() const;

	/**
	 * Sets the normal vectors for vertices in this geometry.
	 * @param normals An array where every 3 values represent the x, y, and z components of a vector.
	 * @param numNormals The number of normals in the array. This is expected to match the number of vertices in this
	 * geometry.
	 */
	void setNormals(const float* normals, unsigned int numNormals);

	/**
	 * Removes the normal vectors for vertices in this geometry, if currently present.
	 */
	void removeNormals();

	/**
	 * Sets the linear RGB color for each vertex in this geometry. Alpha is unencoded.
	 * Convert design-tool sRGB values with Color::fromSRGB before filling this array.
	 * @param colors An array where every 4 values represent the r, g, b, and a components of a color. Each value
	 * should be in the range 0 to 1.
	 * @param numColors The number of colors in the array. This is expected to match the number of
	 * vertices in this geometry.
	 */
	void setColors(const float* colors, unsigned int numColors);

	/**
	 * Removes vertex colors from this geometry, if currently present.
	 */
	void removeColors();

	/**
	 * Sets the texture coordinate for each vertex in this geometry.
	 * @param uvs An array where every 2 values represents the u and v components of a texture coordinate. Coordinates
	 * outside [0, 1] are preserved for texture wrapping.
	 * @param numUVs The number of texture coordinates in the array. This is expected to match the number of
	 * vertices in this geometry.
	 */
	void setTextureCoords(const float* uvs, unsigned int numUVs);

	/**
	 * Removes vertex texture coordinates from this geometry, if currently present.
	 */
	void removeTextureCoords();

	/**
	 * @return An object describing the attributes that make up this geometry
	 */
	const GeometryAttributes getAttributes() const;

	/**
	 * @return Revision of this geometry and its source attributes, useful for consumers that cache GPU geometry.
	 */
	unsigned long long getRevision() const { return _revision; }

	/**
	 * @return The GPU buffer for this geometry, creating it if it does not yet exist. This method should typically only
	 * be called by the renderer.
	 */
	GeometryBuffer* getBuffer();

	/**
	 * @return A newly-constructed GPU buffer for instanced rendering from this geometry. The caller owns the returned buffer.
	 */
	InstanceBuffer* createInstanceBuffer() const;

protected:

	/**
	 * Vertex positions. These positions may be referenced by one or more indices.
	 */
	Vector3* _vertices = nullptr;

	/**
	 * Number of vertex positions.
	 */
	unsigned int _numVertices = 0;

	/**
	 * Ordered vertex indices. These values point to positions in the vertices list.
	 */
	unsigned int* _indices = nullptr;

	/**
	 * Number of vertex indices.
	 */
	unsigned int _numIndices = 0;

	/**
	 * Vertex normals. Can be null, implying no normals specified per-vertex, causing no effect on
	 * lighting. When not empty, the number of normals should equal the number of vertices.
	 */
	Vector3* _normals = nullptr;

	/**
	 * Number of vertex normals
	 */
	unsigned int _numNormals = 0;

	/**
	 * Vertex colors. Can be null, implying no per-vertex coloring. When not empty,
	 * the number of colors should equal the number of vertices.
	 */
	Color* _colors = nullptr;

	/**
	 * Number of colors
	 */
	unsigned int _numColors = 0;

	/**
	 * Texture coordinates. Can be null, implying no per-vertex texture mapping. When not
	 * empty, the number of coordinates should equal the number of vertices.
	 */
	Vector2* _uvs = nullptr;

	/**
	 * Number of texture coordinates.
	 */
	unsigned int _numUVs = 0;

	/**
	 * The GPU buffer for this geometry. Will not be constructed until the first time this geometry is rendered.
	 */
	GeometryBuffer* _buffer = nullptr;

	/**
	 * Constructor
	 * @param type Geometry primitive type
	 */
	Geometry(PrimitiveType type): type(type) {}

	/**
	 * Constructs and stores the GPU buffer for this geometry using the attributes currently set on this geometry
	 */
	void createBuffer();

	/**
	 * Helper method used to build the interleaved floating-point vertex representation used by GPU buffers
	 * @param attributes Geometry attributes
	 * @param numValues Value in which to store the number of values in the result
	 * @param indices Vector in which to store indices, including any tangent discontinuity splits
	 */
	float* createVertexData(const GeometryAttributes& attributes, unsigned int* numValues, std::vector<unsigned int>& indices) const;

	/**
	 * Marks generated tangents and cached GPU geometry as stale and advances the source revision.
	 * Subclasses should call this after changing source attributes directly.
	 */
	void invalidate();

private:

	/**
	 * Recomputes the geometry-owned MikkTSpace tangent data for this geometry if it is out-of-date. Returns
	 * @return Geometry-owned MikkTSpace tangent data, or nullptr for empty/non-triangle geometry or missing basis attributes.
	 * Every 4 values of the result represent tangent direction and handedness for a triangle corner, in draw order. XYZ is the
	 * tangent; W is handedness, with bitangent = W * cross(normal, tangent). A zero tangent with W = 0 marks a face without a
	 * usable tangent basis, and thus its original normal should be used. The pointer must not be freed by the caller and is invalidated by geometry edits.
	 */
	void computeTangents() const;

	/**
	 * Boolean flag indicating whether or not tangent data must be regenerated before its next use
	 *
	 */
	mutable bool _tangentsDirty = true;

	/**
	 * Owned tangent array in draw order. Every four floats store a triangle corner's XYZ direction and handedness.
	 * Contains size() * 4 values when generated; nullptr when no tangent data is available.
	 */
	mutable float* _tangents = nullptr;

	/**
	 * Whether the cached GPU geometry must be rebuilt before its next use.
	 */
	bool _bufferDirty = true;

	/**
	 * Source revision incremented on edits so independent GPU caches can detect stale data.
	 */
	std::uint64_t _revision = 0;
};

/**
 * Triangle-corner source data and output storage shared by MikkTSpace callbacks.
 * @author Nathaniel Rex
 */
struct TangentInput
{
	/**
	 * Source positions expanded, in draw order.
	 */
	std::vector<Vector3> positions;

	/**
	 * Source normals expanded, in draw order.
	 */
	std::vector<Vector3> normals;

	/**
	 * Source texture coordinates expanded, in draw order.
	 */
	std::vector<Vector2> uvs;

	/**
	 * Indices representing the first corner of each face with a usable tangent basis
	 */
	std::vector<unsigned int> faces;
	
	/**
	 * Destination for tangent directions and handedness, in original draw order.
	 */
	float* tangents;

	/**
	 * Maps a filtered MikkTSpace face corner to the original draw order.
	 * @param face Index in the filtered list of usable faces
	 * @param vertex Local triangle corner, from 0 through 2
	 * @return Corner index in the expanded source arrays and tangent output
	 */
	unsigned int corner(int face, int vertex) const { return faces[face] + vertex; }
};