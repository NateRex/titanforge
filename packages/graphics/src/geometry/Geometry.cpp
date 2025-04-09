#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <graphics/core/Color.h>
#include <graphics/core/Buffer.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Assertions.h>
#include <common/Utils.h>
#include <numeric>
#include <cstring>

Geometry::Geometry()
{

}

Geometry::~Geometry()
{
	delete _buffer;
	delete[] _vertices;
	delete[] _indices;
	delete[] _colors;
	delete[] _uvs;

	_vertices = nullptr;
	_indices = nullptr;
	_colors = nullptr;
	_uvs = nullptr;
	_buffer = nullptr;

	_numVertices = 0;
	_numIndices = 0;
	_numColors = 0;
	_numUVs = 0;
}

GeometryPtr Geometry::create()
{
	return std::shared_ptr<Geometry>(new Geometry());
}

void Geometry::setVertices(const float* vertices, unsigned int numVertices)
{
	assertNotNull(vertices, "Vertices cannot be null when applied to a geometry");

	delete[] _vertices;
	_vertices = new Vector3[numVertices];
	_numVertices = numVertices;

	for (int i = 0; i < numVertices; i++)
	{
		int idx = i * 3;
		_vertices[i] = Vector3(vertices[idx], vertices[idx + 1], vertices[idx + 2]);
	}

	// If indices have not been set yet, initialize them to match the number of vertices.
	// They can always be updated later by the caller.
	if (_indices == nullptr)
	{
		_indices = new unsigned int[numVertices];
		_numIndices = numVertices;
		std::iota(_indices, _indices + numVertices, 0);
	}
}

void Geometry::setIndices(const unsigned int* indices, unsigned int numIndices)
{	
	assertNotNull(indices, "Indices cannot be null when applied to a geometry");

	delete[] _indices;
	_indices = new unsigned int[numIndices];
	_numIndices = numIndices;

	std::memcpy(_indices, indices, numIndices * sizeof(unsigned int));
}

unsigned int Geometry::size() const
{
	return _numIndices;
}

void Geometry::setColors(const float* colors, unsigned int numColors)
{
	assertNotNull(colors, "Colors cannot be null when applied to a geometry");

	delete[] _colors;
	_colors = new Color[numColors];
	_numColors = numColors;

	for (int i = 0; i < numColors; i++)
	{
		int idx = i * 4;
		_colors[i] = Color(colors[idx], colors[idx + 1], colors[idx + 2], colors[idx + 3]);
	}
}

void Geometry::removeColors()
{
	delete[] _colors;
	_colors = nullptr;
	_numColors = 0;
}

void Geometry::setTextureCoords(const float* uvs, unsigned int numUVs)
{
	assertNotNull(uvs, "Texture coordinates cannot be null when applied to a geometry");

	delete[] _uvs;
	_uvs = new Vector2[numUVs];
	_numUVs = numUVs;

	for (int i = 0; i < numUVs; i++)
	{
		int idx = i * 2;

		assertInRange(uvs[i], 0.f, 1.f, true, "UV coordinate must be between 0 and 1");
		assertInRange(uvs[i + 1], 0.f, 1.f, true, "UV coordinate must be between 0 and 1");

		_uvs[i] = Vector2(uvs[idx], uvs[idx + 1]);
	}
}

void Geometry::removeTextureCoords()
{
	delete[] _uvs;
	_uvs = nullptr;
	_numUVs = 0;
}

const GeometryAttributes Geometry::getAttributes() const
{
	return {
		_colors != nullptr,
		_uvs != nullptr
	};
}

Buffer* Geometry::getBuffer()
{
	if (_buffer == nullptr)
	{
		createBuffer();
	}

	return _buffer;
}

void Geometry::createBuffer()
{
	GeometryAttributes attribs = getAttributes();
	assertNotNull(_vertices, "Geometry must contain vertex positions to render");
	assertNotNull(_indices, "Geometry must contain vertex indices to render");
	assertTrue(!attribs.colors || _numColors == _numVertices, "Number of colors on a geometry must match the number of vertices");
	assertTrue(!attribs.uvs || _numUVs == _numVertices, "Number of texture coordinates on a geometry must match the number of vertices");

	// Interleave vertex data
	unsigned int vSize = _numVertices * attribs.getStride();
	float* vData = new float[vSize];
	unsigned int idx = 0;
	for (int i = 0; i < _numVertices; i++)
	{
		Vector3 p = _vertices[i];
		vData[idx++] = p.x;
		vData[idx++] = p.y;
		vData[idx++] = p.z;

		if (attribs.colors)
		{
			Color c = _colors[i];
			vData[idx++] = c.red();
			vData[idx++] = c.green();
			vData[idx++] = c.blue();
			vData[idx++] = c.alpha();
		}

		if (attribs.uvs)
		{
			Vector2 uv = _uvs[i];
			vData[idx++] = uv.x;
			vData[idx++] = uv.y;
		}
	}

	delete _buffer;
	_buffer = new Buffer(attribs, vData, vSize, _indices, _numIndices);

	delete[] vData;
}