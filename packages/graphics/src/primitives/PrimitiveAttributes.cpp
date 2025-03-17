#include <graphics/primitives/PrimitiveAttributes.h>

PrimitiveAttributes::PrimitiveAttributes()
	: hasColor(false), hasTextureCoords(false)
{

}

PrimitiveAttributes::PrimitiveAttributes(bool hasColor, bool hasTextureCoords)
	: hasColor(hasColor), hasTextureCoords(hasTextureCoords)
{

}

bool PrimitiveAttributes::operator==(const PrimitiveAttributes& other) const
{
	return hasColor == other.hasColor
		&& hasTextureCoords == other.hasTextureCoords;
}

bool PrimitiveAttributes::operator!=(const PrimitiveAttributes& other) const
{
	return !(*this == other);
}