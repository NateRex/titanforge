#include <graphics/primitives/Primitive.h>

// ------------------------------------------------------------------------------------------------------------------
// VertexAttributes
// ------------------------------------------------------------------------------------------------------------------

int VertexAttributes::getStride() const
{
	int stride = 3;

	if (hasColor)
	{
		stride += 4;
	}

	if (hasTexture)
	{
		stride += 2;
	}

	return stride;
}

bool VertexAttributes::operator==(const VertexAttributes& other) const
{
	return hasColor == other.hasColor
		&& hasTexture == other.hasTexture;
}

bool VertexAttributes::operator!=(const VertexAttributes& other) const
{
	return !(*this == other);
}


// ------------------------------------------------------------------------------------------------------------------
// Polyface
// ------------------------------------------------------------------------------------------------------------------


Primitive::Primitive(const VertexAttributes& vertexAttributes) : _vertexAttributes(vertexAttributes)
{

}

const VertexAttributes Primitive::getVertexAttributes() const
{
	return _vertexAttributes;
}