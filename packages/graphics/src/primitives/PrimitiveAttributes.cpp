#include <graphics/primitives/PrimitiveAttributes.h>

PrimitiveAttributes::PrimitiveAttributes(bool hasColor) : hasColor(hasColor)
{

}

bool PrimitiveAttributes::operator==(const PrimitiveAttributes& other) const
{
	return hasColor == other.hasColor;
}

bool PrimitiveAttributes::operator!=(const PrimitiveAttributes& other) const
{
	return !(*this == other);
}