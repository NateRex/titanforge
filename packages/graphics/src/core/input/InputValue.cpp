#include <graphics/core/input/InputValue.h>

InputValue::InputValue(bool value) : _type(InputValueType::BOOLEAN), _boolValue(value)
{

}

InputValue::InputValue(float value) : _type(InputValueType::SCALAR), _scalarValue(value)
{

}

InputValue::InputValue(const Vector2& value) : _type(InputValueType::VECTOR_2D), _vector2Value(value)
{

}

InputValue::InputValue(const Vector3& value) : _type(InputValueType::VECTOR_3D), _vector3Value(value)
{

}

InputValue::InputValue(const InputValue& other) : _type(other._type), _vector3Value(other._vector3Value)
{

}