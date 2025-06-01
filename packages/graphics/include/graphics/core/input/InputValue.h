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
	 * The type of input data held by this object.
	 */
	const InputValueType type;

	/**
	 * Constructs an input action value containing boolean input
	 * @param value Input value
	 */
	InputValue(bool value);

	/**
	 * Constructs an input action value containing scalar input
	 * @param value Input value
	 */
	InputValue(float scalar);

	/**
	 * Constructs an input action value containing 2D vector data
	 * @param value Input value
	 */
	InputValue(const Vector2& value);

	/**
	 * Constructs an input action value containing 3D vector data
	 * @param value Input value
	 */
	InputValue(const Vector3& value);

	/**
	 * @return The boolean value stored by this object
	 */
	bool getBoolean() const;

	/**
	 * @return The scalar value stored by this object
	 */
	float getScalar() const;

	/**
	 * @return The 2D vector value stored by this object
	 */
	Vector2 get2D() const;

	/**
	 * @return The 3D vector value stored by this object
	 */
	Vector3 get3D() const;

private:

	/**
	 * The data stored by this object
	 */
	union {
		bool _boolValue;
		float _scalarValue;
		Vector2 _vector2Value;
		Vector3 _vector3Value;
	};
};