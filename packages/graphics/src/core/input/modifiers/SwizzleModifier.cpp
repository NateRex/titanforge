#include <graphics/core/input/modifiers/SwizzleModifier.h>
#include <math/Axis.h>
#include <graphics/core/input/InputValue.h>

SwizzleModifier::SwizzleModifier(Axis first, Axis second, Axis third)
{
	_order[0] = first;
	_order[1] = second;
	_order[2] = third;
}

InputValue SwizzleModifier::apply(const InputValue& value) const
{
	switch (value.getType())
	{
		case InputValueType::BOOLEAN:
		{
			bool b = value.getBoolean();
			return InputValue(_order[0] == Axis::X ? b : false);
		}
		case InputValueType::SCALAR:
		{
			float s = value.getScalar();
			return InputValue(_order[0] == Axis::X ? s : false);
		}
		case InputValueType::VECTOR_2D:
		{
			Vector2 v = value.get2D();
			float vArray[2]{ v.x, v.y };
			return InputValue(
				_order[0] != Axis::Z ? vArray[static_cast<int>(_order[0])] : 0.f,
				_order[1] != Axis::Z ? vArray[static_cast<int>(_order[1])] : 0.f
			);
		}
		case InputValueType::VECTOR_3D:
		{
			Vector3 v = value.get3D();
			float vArray[3]{ v.x, v.y, v.z };
			return InputValue(
				vArray[static_cast<int>(_order[0])],
				vArray[static_cast<int>(_order[1])],
				vArray[static_cast<int>(_order[2])]
			);
		}
		default:
			return value;
	}
}