#pragma once
#include <graphics/core/input/InputValueType.h>
#include <math/Vector2.h>
#include <math/Vector3.h>

/**
 * This class encapsulates the input data (bool, float, Vector2D, or Vector3D) for an input action.
 * It allows querying the correct type at runtime in bound callbacks.
 * @author Nathaniel Rex
 */
class InputValue
{
public:

	/**
	 * Constructs an input action value containing boolean data
	 * @param value Input value
	 */
	InputValue(bool value);

	/**
	 * Constructs an input action value containing scalar data
	 * @param value Input value
	 */
	InputValue(float scalar);

	/**
	 * Constructs an input action value containing 2D vector data
	 * @param x The x component value
	 * @param y The y component value
	 */
	InputValue(float x, float y);

	/**
	 * Constructs an input action value containing 3D vector data
	 * @param x The x component value
	 * @param y The y component value
	 * @param z The z component value
	 */
	InputValue(float x, float y, float z);

	/**
	 * Constructs an input action value from an existing value
	 * @param other Input action value to copy from
	 */
	InputValue(const InputValue& other);

	/**
	 * @return The type of data held by this object
	 */
	InputValueType getType() const { return _type; }

	/**
	 * @return The boolean value stored by this object
	 */
	bool getBoolean() const { return _data.b; }

	/**
	 * @return The scalar value stored by this object
	 */
	float getScalar() const { return _data.s; }

	/**
	 * @return The 2D vector value stored by this object
	 */
	Vector2 get2D() const { return Vector2(_data.v2[0], _data.v2[1]); }

	/**
	 * @return The 3D vector value stored by this object
	 */
	Vector3 get3D() const { return Vector3(_data.v3[0], _data.v3[1], _data.v3[2]); }

private:

	/**
	 * The type of data held by this object.
	 */
	InputValueType _type;

	/**
	 * The data stored by this object
	 */
	union {
		bool b;
		float s;
		float v2[2];
		float v3[3];
	} _data;
};