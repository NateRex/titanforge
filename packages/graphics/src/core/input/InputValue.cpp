#include <graphics/core/input/InputValue.h>
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

InputValue::InputValue(const InputValue& other) : _type(other._type)
{
	std::memcpy(&_data, &other._data, sizeof(_data));
}