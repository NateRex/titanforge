#include <graphics/geometry/Geometry.h>
#include <graphics/core/Color.h>
#include <graphics/core/buffers/GeometryBuffer.h>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/Assertions.h>
#include <common/Utils.h>
#include <glad/glad.h>
#include <mikktspace.h>
#include <numeric>
#include <cmath>
#include <map>
#include <array>
#include <algorithm>
#include <cstring>

unsigned int toGLPrimitive(PrimitiveType type)
{
	switch (type)
	{
		case PrimitiveType::POINTS: return GL_POINTS;
		case PrimitiveType::LINE_SEGMENTS: return GL_LINES;
		case PrimitiveType::POLYLINE: return GL_LINE_STRIP;
		case PrimitiveType::CLOSED_POLYLINE: return GL_LINE_LOOP;
		case PrimitiveType::TRIANGLES: return GL_TRIANGLES;
		default: return GL_POINTS;
	}
}

Geometry::~Geometry()
{
	delete _buffer;
	delete[] _vertices;
	delete[] _indices;
	delete[] _normals;
	delete[] _colors;
	delete[] _uvs;
	delete[] _tangents;

	_vertices = nullptr;
	_indices = nullptr;
	_normals = nullptr;
	_colors = nullptr;
	_uvs = nullptr;
	_tangents = nullptr;
	_buffer = nullptr;

	_numVertices = 0;
	_numIndices = 0;
	_numNormals = 0;
	_numColors = 0;
	_numUVs = 0;
}

GeometryPtr Geometry::create(PrimitiveType primitiveType)
{
	return std::shared_ptr<Geometry>(new Geometry(primitiveType));
}

void Geometry::setVertices(const float* vertices, unsigned int numVertices, bool updateIndices)
{
	assertNotNull(vertices, "Vertices cannot be null when applied to a geometry");

	invalidate();
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
	if (_indices == nullptr && updateIndices)
	{
		_indices = new unsigned int[numVertices];
		_numIndices = numVertices;
		std::iota(_indices, _indices + numVertices, 0);
	}
}

void Geometry::setIndices(const unsigned int* indices, unsigned int numIndices)
{	
	assertNotNull(indices, "Indices cannot be null when applied to a geometry");

	invalidate();
	delete[] _indices;
	_indices = new unsigned int[numIndices];
	_numIndices = numIndices;

	std::memcpy(_indices, indices, numIndices * sizeof(unsigned int));
}

unsigned int Geometry::size() const
{
	return _indices != nullptr ? _numIndices : _numVertices;
}

void Geometry::setNormals(const float* normals, unsigned int numNormals)
{
	assertNotNull(normals, "Normals cannot be null when applied to a geometry");

	invalidate();
	delete[] _normals;
	_normals = new Vector3[numNormals];
	_numNormals = numNormals;

	for (int i = 0; i < numNormals; i++)
	{
		int idx = i * 3;
		_normals[i] = Vector3(normals[idx], normals[idx + 1], normals[idx + 2]);
	}
}

void Geometry::removeNormals()
{
	invalidate();
	delete[] _normals;
	_normals = nullptr;
	_numNormals = 0;
}

void Geometry::setColors(const float* colors, unsigned int numColors)
{
	assertNotNull(colors, "Colors cannot be null when applied to a geometry");

	invalidate();
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
	invalidate();
	delete[] _colors;
	_colors = nullptr;
	_numColors = 0;
}

void Geometry::setTextureCoords(const float* uvs, unsigned int numUVs)
{
	assertNotNull(uvs, "Texture coordinates cannot be null when applied to a geometry");

	invalidate();
	delete[] _uvs;
	_uvs = new Vector2[numUVs];
	_numUVs = numUVs;

	for (int i = 0; i < numUVs; i++)
	{
		int idx = i * 2;

		_uvs[i] = Vector2(uvs[idx], uvs[idx + 1]);
	}
}

void Geometry::removeTextureCoords()
{
	invalidate();
	delete[] _uvs;
	_uvs = nullptr;
	_numUVs = 0;
}

const GeometryAttributes Geometry::getAttributes() const
{
	return {
		type,
		_indices != nullptr,
		_normals != nullptr,
		_colors != nullptr,
		_uvs != nullptr,
		type == PrimitiveType::TRIANGLES && _normals != nullptr && _uvs != nullptr
	};
}

GeometryBuffer* Geometry::getBuffer()
{
	if (_buffer == nullptr || _bufferDirty)
	{
		createBuffer();
	}

	return _buffer;
}

void Geometry::createBuffer()
{
	GeometryAttributes attributes = getAttributes();
	unsigned int vSize = 0;
	std::vector<unsigned int> indices;
	float* vData = createVertexData(attributes, &vSize, indices);

	GeometryBuffer* replacement = new GeometryBuffer(attributes, vData, vSize, attributes.indices ? indices.data() : nullptr, indices.size());
	delete _buffer;
	_buffer = replacement;
	_bufferDirty = false;
	delete[] vData;
}

InstanceBuffer* Geometry::createInstanceBuffer() const
{
	GeometryAttributes attributes = getAttributes();
	unsigned int numValues = 0;
	std::vector<unsigned int> indices;
	float* vertexData = createVertexData(attributes, &numValues, indices);

	InstanceBuffer* buffer = new InstanceBuffer(attributes, vertexData, numValues, attributes.indices ? indices.data() : nullptr, indices.size());
	delete[] vertexData;
	return buffer;
}

float* Geometry::createVertexData(const GeometryAttributes& attributes, unsigned int* numValues, std::vector<unsigned int>& indices) const
{
	assertNotNull(_vertices, "Geometry must contain vertex positions");
	assertTrue(!attributes.normals || _numNormals == _numVertices, "Number of vertex normals must match the number of vertices");
	assertTrue(!attributes.colors || _numColors == _numVertices, "Number of colors must match the number of vertices");
	assertTrue(!attributes.uvs || _numUVs == _numVertices, "Number of texture coordinates must match the number of vertices");

	for (unsigned int i = 0; i < _numIndices; ++i)
	{
		assertTrue(_indices[i] < _numVertices, "Geometry index is out of range");
	}

	if (_indices && !attributes.tangents)
	{
		indices.assign(_indices, _indices + _numIndices);
	}

	computeTangents();
	const unsigned int count = attributes.tangents ? size() : _numVertices;
	*numValues = count * attributes.getStride();

	float* vData = new float[*numValues];
	unsigned int idx = 0;

	std::map<std::pair<unsigned int, std::array<float, 4>>, unsigned int> vertices;
	for (unsigned int corner = 0; corner < count; ++corner)
	{
		const unsigned int i = attributes.tangents && _indices ? _indices[corner] : corner;
		if (attributes.tangents && _indices)
		{
			const float* tangent = _tangents + corner * 4;
			std::array<float, 4> frame = { tangent[0], tangent[1], tangent[2], tangent[3] };

			auto entry = vertices.emplace(std::make_pair(i, frame), static_cast<unsigned int>(vertices.size()));
			indices.push_back(entry.first->second);
			if (!entry.second) continue;
		}
		Vector3 p = _vertices[i];
		vData[idx++] = p.x;
		vData[idx++] = p.y;
		vData[idx++] = p.z;

		if (attributes.normals)
		{
			Vector3 n = _normals[i];
			vData[idx++] = n.x;
			vData[idx++] = n.y;
			vData[idx++] = n.z;
		}

		if (attributes.colors)
		{
			Color c = _colors[i];
			vData[idx++] = c.red();
			vData[idx++] = c.green();
			vData[idx++] = c.blue();
			vData[idx++] = c.alpha();
		}

		if (attributes.uvs)
		{
			Vector2 uv = _uvs[i];
			vData[idx++] = uv.x;
			vData[idx++] = uv.y;
		}

		if (attributes.tangents)
		{
			for (unsigned int component = 0; component < 4; component++)
			{
				vData[idx++] = _tangents[corner * 4 + component];
			}
		}
	}

	*numValues = idx;
	return vData;
}

void Geometry::invalidate()
{
	_tangentsDirty = true;
	_bufferDirty = true;
	++_revision;
}

void Geometry::computeTangents() const
{
	if (!_tangentsDirty) return;

	delete[] _tangents;
	_tangents = nullptr;

	if (size() == 0 || type != PrimitiveType::TRIANGLES || !_vertices || !_normals || !_uvs || _numNormals != _numVertices || _numUVs != _numVertices)
	{
		_tangentsDirty = false;
	}

	TangentInput input;
	_tangents = new float[static_cast<std::size_t>(size()) * 4]();
	input.tangents = _tangents;
	for (unsigned int corner = 0; corner < size(); ++corner)
	{
		unsigned int vertex = _indices ? _indices[corner] : corner;
		assertTrue(vertex < _numVertices, "Geometry index is out of range");
		input.positions.push_back(_vertices[vertex]);
		input.normals.push_back(_normals[vertex]);
		input.uvs.push_back(_uvs[vertex]);
	}

	for (unsigned int face = 0; face + 2 < size(); face += 3)
	{
		bool valid = true;
		for (unsigned int i = face; i < face + 3; ++i)
		{
			const auto& p = input.positions[i];
			const auto& n = input.normals[i];
			const auto& uv = input.uvs[i];
			valid = valid && std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z)
				&& std::isfinite(n.x) && std::isfinite(n.y) && std::isfinite(n.z)
				&& (n.x != 0 || n.y != 0 || n.z != 0) && std::isfinite(uv.x) && std::isfinite(uv.y);
		}

		const auto& a = input.uvs[face];
		const auto& b = input.uvs[face + 1];
		const auto& c = input.uvs[face + 2];
		const double determinant = (double(b.x) - a.x) * (double(c.y) - a.y)
			- (double(b.y) - a.y) * (double(c.x) - a.x);
		const Vector3 edge1 = input.positions[face + 1].minus(input.positions[face]);
		const Vector3 edge2 = input.positions[face + 2].minus(input.positions[face]);
		const Vector3 area = edge1.cross(edge2);
		if (valid && determinant != 0 && (area.x != 0 || area.y != 0 || area.z != 0)) input.faces.push_back(face);
	}

	SMikkTSpaceInterface callbacks = {};
	callbacks.m_getNumFaces = [](const SMikkTSpaceContext* c) { return int(static_cast<TangentInput*>(c->m_pUserData)->faces.size()); };
	callbacks.m_getNumVerticesOfFace = [](const SMikkTSpaceContext*, int) { return 3; };
	callbacks.m_getPosition = [](const SMikkTSpaceContext* c, float* out, int face, int vertex)
	{
		const auto& input = *static_cast<TangentInput*>(c->m_pUserData);
		const auto& p = input.positions[input.corner(face, vertex)];
		out[0] = p.x; out[1] = p.y; out[2] = p.z;
	};
	callbacks.m_getNormal = [](const SMikkTSpaceContext* c, float* out, int face, int vertex)
	{
		const auto& input = *static_cast<TangentInput*>(c->m_pUserData);
		const auto& n = input.normals[input.corner(face, vertex)];
		out[0] = n.x; out[1] = n.y; out[2] = n.z;
	};
	callbacks.m_getTexCoord = [](const SMikkTSpaceContext* c, float* out, int face, int vertex)
	{
		const auto& input = *static_cast<TangentInput*>(c->m_pUserData);
		const auto& uv = input.uvs[input.corner(face, vertex)];
		out[0] = uv.x; out[1] = uv.y;
	};
	callbacks.m_setTSpaceBasic = [](const SMikkTSpaceContext* c, const float* tangent, float sign, int face, int vertex)
	{
		auto& input = *static_cast<TangentInput*>(c->m_pUserData);
		if (std::isfinite(tangent[0]) && std::isfinite(tangent[1]) && std::isfinite(tangent[2]))
		{
			float* output = input.tangents + input.corner(face, vertex) * 4;
			std::copy(tangent, tangent + 3, output);
			output[3] = sign;
		}
	};

	SMikkTSpaceContext context = { &callbacks, &input };
	if (!input.faces.empty() && !genTangSpaceDefault(&context))
	{
		std::fill_n(_tangents, static_cast<std::size_t>(size()) * 4, 0.f);
	}
	
	_tangentsDirty = false;
}
