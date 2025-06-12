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
			v.scale(_negateX ? -1.f : 1.f, _negateY ? -1.f : 1.f, &v);
			return InputValue(v);
		}
		case InputValueType::VECTOR_3D:
		{
			Vector3 v = value.get3D();
			v.scale(_negateX ? -1.f : 1.f, _negateY ? -1.f : 1.f, _negateZ ? -1.f : 1.f, &v);
			return InputValue(v);
		}
		default:
			return value;
	}
}