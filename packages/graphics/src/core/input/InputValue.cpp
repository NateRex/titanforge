#include <graphics/core/input/InputValue.h>

InputValue::InputValue(bool value) : type(InputValueType::BOOLEAN), _boolValue(value)
{

}

InputValue::InputValue(float value) : type(InputValueType::SCALAR), _scalarValue(value)
{

}

InputValue::InputValue(const Vector2& value) : type(InputValueType::VECTOR_2D), _vector2Value(value)
{

}

InputValue::InputValue(const Vector3& value) : type(InputValueType::VECTOR_3D), _vector3Value(value)
{

}

bool InputValue::getBoolean() const
{
	return _boolValue;
}

float InputValue::getScalar() const
{
	return _scalarValue;
}

Vector2 InputValue::get2D() const
{
	return _vector2Value;
}

Vector3 InputValue::get3D() const
{
	return _vector3Value;
}