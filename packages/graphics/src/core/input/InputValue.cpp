#include <graphics/core/input/InputValue.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Assertions.h>
#include <cstring>

InputValue::InputValue(bool value) : _type(InputValueType::BOOLEAN)
{
	std::memset(&_data, 0, sizeof(_data));
	_data.b = value;
}

InputValue::InputValue(float value) : _type(InputValueType::SCALAR)
{
	std::memset(&_data, 0, sizeof(_data));
	_data.s = value;
}

InputValue::InputValue(float x, float y) : _type(InputValueType::VECTOR_2D)
{
	std::memset(&_data, 0, sizeof(_data));
	_data.v2[0] = x;
	_data.v2[1] = y;
}

InputValue::InputValue(float x, float y, float z) : _type(InputValueType::VECTOR_3D)
{
	std::memset(&_data, 0, sizeof(_data));
	_data.v3[0] = x;
	_data.v3[1] = y;
	_data.v3[2] = z;
}

InputValue::InputValue(InputValueType type, float x, float y, float z) : _type(type)
{
	std::memset(&_data, 0, sizeof(_data));
	switch (type)
	{
		case InputValueType::BOOLEAN:
			_data.b = (x != 0);
			break;
		case InputValueType::SCALAR:
			_data.s = x;
			break;
		case InputValueType::VECTOR_2D:
			_data.v2[0] = x;
			_data.v2[1] = y;
			break;
		case InputValueType::VECTOR_3D:
		default:
			_data.v3[0] = x;
			_data.v3[1] = y;
			_data.v3[2] = z;
			break;
	}
}

InputValue::InputValue(const InputValue& other) : _type(other._type)
{
	std::memcpy(&_data, &other._data, sizeof(_data));
}

bool InputValue::getBoolean() const
{
	assertTrue(_type == InputValueType::BOOLEAN, "Input value does not contain boolean");
	return _data.b;
}

float InputValue::getScalar() const
{
	assertTrue(_type == InputValueType::SCALAR, "Input value does not contain a scalar");
	return _data.s;
}

Vector2 InputValue::get2D() const
{
	assertTrue(_type == InputValueType::VECTOR_2D, "Input value does not contain a 2D vector");
	return Vector2(_data.v2[0], _data.v2[1]);
}

Vector3 InputValue::get3D() const
{
	assertTrue(_type == InputValueType::VECTOR_3D, "Input value does not contain a 3D vector");
	return Vector3(_data.v3[0], _data.v3[1], _data.v3[2]);
}