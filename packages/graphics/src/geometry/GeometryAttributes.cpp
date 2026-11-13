#include <graphics/geometry/GeometryAttributes.h>

int GeometryAttributes::getStride() const
{
	int stride = 3;

	if (normals)
	{
		stride += 3;
	}

	if (colors)
	{
		stride += 4;
	}

	if (uvs)
	{
		stride += 2;
	}

	if (tangents) stride += 4;

	return stride;
}

bool GeometryAttributes::operator==(const GeometryAttributes& other) const
{
	return primitiveType == other.primitiveType
		&& indices == other.indices
		&& normals == other.normals
		&& colors == other.colors
		&& uvs == other.uvs
		&& tangents == other.tangents;
}

bool GeometryAttributes::operator!=(const GeometryAttributes& other) const
{
	return !(*this == other);
}