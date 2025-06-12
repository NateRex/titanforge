#include <graphics/core/input/modifiers/NegateModifier.h>
#include <graphics/core/input/InputValue.h>

NegateModifier::NegateModifier(bool negateX, bool negateY, bool negateZ)
	: _negateX(negateX), _negateY(negateY), _negateZ(negateZ)
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