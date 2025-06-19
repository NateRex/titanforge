#include <graphics/core/input/modifiers/NegateModifier.h>
#include <graphics/core/input/InputValue.h>
#include <math/Vector2.h>
#include <math/Vector3.h>

NegateModifier::NegateModifier(bool x, bool y, bool z)
	: _negateX(x), _negateY(y), _negateZ(z)
{

}

InputValue NegateModifier::apply(const InputValue& value) const
{
	switch (value.getType())
	{
		case InputValueType::BOOLEAN:
		{
			bool b = value.getBoolean();
			return InputValue(_negateX ? !b : b);
		}
		case InputValueType::SCALAR:
		{
			float s = value.getScalar();
			return InputValue(_negateX ? -s : s);
		}
		case InputValueType::VECTOR_2D:
		{
			Vector2 v = value.get2D();
			return InputValue(
				_negateX ? -v.x : v.x,
				_negateY ? -v.y : v.y
			);
		}
		case InputValueType::VECTOR_3D:
		{
			Vector3 v = value.get3D();
			return InputValue(
				_negateX ? -v.x : v.x,
				_negateY ? -v.y : v.y,
				_negateZ ? -v.z : v.z
			);
		}
		default:
			return value;
	}
}