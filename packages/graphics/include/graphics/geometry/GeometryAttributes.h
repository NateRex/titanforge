#pragma once

/**
 * A class describing all of the attributes that are present on a geometry
 * @author Nathaniel Rex
 */
class GeometryAttributes
{
public:

	/**
	 * Flag that, when true, indicates that the geometry contains per-vertex normals. Defaults to false.
	 */
	bool normals = false;

	/**
	 * Flag that, when true, indicates that the geometry contains per-vertex color values. Defaults to false.
	 */
	bool colors = false;

	/**
	 * Flag that, when true, indicates that the geometry contains per-vertex texture coordinates. Defaults to false.
	 */
	bool uvs = false;

	/**
	 * @return The number of floating point values used to represent a single vertex in the geometry, taking into account
	 * all attributes.
	 */
	int getStride() const;

	/**
	 * Operator that compares this set of attributes to another and tests for equality
	 * @param other The set of attributes to compare to
	 * @return True if this set of geometry attributes are equal to the one given. Returns false otherwise.
	 */
	bool operator==(const GeometryAttributes& other) const;

	/**
	 * Operator that compares this set of attributes to another and tests for inequality
	 * @param other The set of attributes to compare to
	 * @return True if this set of geometry attributes are not equal to the one given. Returns false otherwise.
	 */
	bool operator!=(const GeometryAttributes& other) const;
};