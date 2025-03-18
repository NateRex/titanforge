#pragma once

/**
 * Set of flags used to determine what data is present inside of a primitive.
 * @author Nathaniel Rex
 */
class PrimitiveAttributes
{
public:

	/**
	 * Flag that, when true, indicates that the primitive contains color values
	 */
	bool hasColor;

	/**
	 * Flag that, when true, indicates that the primitive contains texture coordinates
	 */
	bool hasTextureCoords;

	/**
	 * Constructor. All attribute flags will be initially set to false.
	 */
	PrimitiveAttributes();

	/**
	 * Constructor
	 * @param hasColor True if the primitive contains color data. False otherwise.
	 * @param hasTextureCoords True if the primitive contains texture coordinate data. False otherwise.
	 */
	PrimitiveAttributes(bool hasColor, bool hasTextureCoords);

	/**
	 * Operator that compares this set of attributes to another, and tests for equality
	 * @param other The set of attributes to compare to
	 * @return True if this set of primitive attributes is equal to the one given. Returns false otherwise.
	 */
	bool operator==(const PrimitiveAttributes& other) const;

	/**
	 * Operator that compares this set of attributes to another, and tests for inequality
	 * @param other The set of attributes to compare to
	 * @return True if this set of primitive attributes is not equal to the one given. Returns false otherwise.
	 */
	bool operator!=(const PrimitiveAttributes& other) const;
};