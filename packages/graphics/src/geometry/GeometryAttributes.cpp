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

	return stride;
}

bool GeometryAttributes::operator==(const GeometryAttributes& other) const
{
	return normals == other.normals
		&& colors == other.colors
		&& uvs == other.uvs;
}

bool GeometryAttributes::operator!=(const GeometryAttributes& other) const
{
	return !(*this == other);
}