#include <boost/test/unit_test.hpp>
#include <graphics/geometry/Geometry.h>
#include <graphics/core/buffers/GeometryBuffer.h>
#include <glad/glad.h>
#include <limits>
#include <array>

/**
 * Reads uploaded tangent values in draw order through the geometry buffer's GL attributes and indices.
 * @param geometry Geometry whose GPU upload is inspected
 * @return Four tangent components per drawn corner
 */
std::vector<float> readUploadedTangents(const GeometryPtr& geometry)
{
	GeometryBuffer* buffer = geometry->getBuffer();
	buffer->bind();
	GLint enabled = 0, vbo = 0, stride = 0;
	void* offset = nullptr;
	glGetVertexAttribiv(8, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
	BOOST_REQUIRE(enabled == GL_TRUE);
	glGetVertexAttribiv(8, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo);
	glGetVertexAttribiv(8, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
	glGetVertexAttribPointerv(8, GL_VERTEX_ATTRIB_ARRAY_POINTER, &offset);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	std::vector<float> tangents(buffer->size() * 4);
	for (unsigned int corner = 0; corner < buffer->size(); ++corner)
	{
		unsigned int vertex = corner;
		if (buffer->getGeometryAttributes().indices)
			glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, corner * sizeof(unsigned int), sizeof(vertex), &vertex);
		glGetBufferSubData(GL_ARRAY_BUFFER, reinterpret_cast<GLintptr>(offset) + vertex * stride,
			4 * sizeof(float), tangents.data() + corner * 4);
	}
	return tangents;
}

/**
 * Tests that a geometry is initially empty on construction
 */
BOOST_AUTO_TEST_CASE(Geometry_construction)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);
	BOOST_TEST(geom->size() == 0);

	GeometryAttributes attributes = geom->getAttributes();
	BOOST_TEST(!attributes.normals);
	BOOST_TEST(!attributes.colors);
	BOOST_TEST(!attributes.uvs);
}

/**
 * Tests the ability to set the vertices and indices of a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_verticesAndIndices)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float vertices[] = {
		0.f, 0.f, 0.f,
		1.f, 2.f, 0.f,
		2.f, 0.f, 0.f 
	};

	// Set vertices, no indices
	geom->setVertices(vertices, 3, false);
	BOOST_TEST(geom->size() == 3);

	// Set vertices and indices together
	geom->setVertices(vertices, 3);
	BOOST_TEST(geom->size() == 3);

	// Update indices so that now there are 4 vertices in total
	unsigned int indices[] = { 0, 1, 2, 0 };
	geom->setIndices(indices, 4);
	BOOST_TEST(geom->size() == 4);
}

/**
 * Tests the ability to add and remove vertex normals from a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_normals)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float normals[] = {
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		1.f, 1.f, 1.f
	};
	geom->setNormals(normals, 3);
	BOOST_TEST(geom->getAttributes().normals);

	geom->removeNormals();
	BOOST_TEST(!geom->getAttributes().normals);
}

/**
 * Tests the ability to add and remove colors from a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_colors)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float colors[] = {
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 0.f
	};
	geom->setColors(colors, 4);
	BOOST_TEST(geom->getAttributes().colors);

	geom->removeColors();
	BOOST_TEST(!geom->getAttributes().colors);
}

/**
 * Tests the ability to add and remove texture coordinates from a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_uvs)
{
	GeometryPtr geom = Geometry::create(PrimitiveType::TRIANGLES);

	float uvs[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 0.f };
	geom->setTextureCoords(uvs, 3);
	BOOST_TEST(geom->getAttributes().uvs);

	geom->removeTextureCoords();
	BOOST_TEST(!geom->getAttributes().uvs);
}

/**
 * Tests the ability to construct a GL buffer containing the data for a geometry
 */
BOOST_AUTO_TEST_CASE(Geometry_buffer)
{
	GeometryPtr geometry = Geometry::create(PrimitiveType::TRIANGLES);
	float vertices[] = {
		0.f, 0.f, 0.f,
		1.f, 2.f, 0.f,
		2.f, 0.f, 0.f
	};
	geometry->setVertices(vertices, 3);

	GeometryBuffer* buffer = geometry->getBuffer();
	BOOST_TEST(buffer != nullptr);
	BOOST_TEST(buffer->size() == geometry->size());
}

/**
 * Tests tangent direction and handedness for indexed and non-indexed triangles.
 */
BOOST_AUTO_TEST_CASE(Geometry_tangentOrientation)
{
	const float positions[] = { 0,0,0, 1,0,0, 0,1,0 };
	const float normals[] = { 0,0,1, 0,0,1, 0,0,1 };
	const float uvs[] = { 0,0, 1,0, 0,1 };
	for (bool indexed : { false, true })
	{
		auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
		geometry->setVertices(positions, 3, indexed);
		geometry->setNormals(normals, 3);
		geometry->setTextureCoords(uvs, 3);
		const auto tangents = readUploadedTangents(geometry);
		for (unsigned int corner = 0; corner < geometry->size(); ++corner)
		{
			const float* tangent = tangents.data() + corner * 4;
			BOOST_TEST(tangent[0] == 1.f, boost::test_tools::tolerance(0.0001f));
			BOOST_TEST(tangent[1] == 0.f, boost::test_tools::tolerance(0.0001f));
			BOOST_TEST(tangent[2] == 0.f, boost::test_tools::tolerance(0.0001f));
			BOOST_TEST(tangent[3] == 1.f);
		}
	}
}

/**
 * Tests original-normal fallback markers for degenerate UVs and absent tangent data for missing UVs.
 */
BOOST_AUTO_TEST_CASE(Geometry_invalidTangentBasis)
{
	const float positions[] = { 0,0,0, 1,0,0, 0,1,0 };
	const float normals[] = { 0,0,1, 0,0,1, 0,0,1 };
	auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
	geometry->setVertices(positions, 3);
	geometry->setNormals(normals, 3);
	const float degenerate[] = { 0,0, 0,0, 0,0 };
	geometry->setTextureCoords(degenerate, 3);
	const auto tangents = readUploadedTangents(geometry);
	for (unsigned int corner = 0; corner < geometry->size(); ++corner) BOOST_TEST(tangents[corner * 4 + 3] == 0.f);
	geometry->removeTextureCoords();
	BOOST_TEST(!geometry->getBuffer()->getGeometryAttributes().tangents);
}

/**
 * Tests mirrored handedness, vertex splits, preserved colors, and the uploaded tangent attribute layout.
 */
BOOST_AUTO_TEST_CASE(Geometry_mirroredTangentsAndUpload)
{
	// Two CCW triangles share vertex 0 but have opposite UV orientation.
	const float positions[] = { 0,0,0, 1,0,0, 0,1,0, -1,0,0 };
	const float normals[] = { 0,0,1, 0,0,1, 0,0,1, 0,0,1 };
	const float uvs[] = { 0,0, 1,0, 0,1, 1,0 };
	const float colors[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1, 1,1,0,1 };
	const unsigned int indices[] = { 0,1,2, 0,2,3 };
	auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
	geometry->setVertices(positions, 4);
	geometry->setIndices(indices, 6);
	geometry->setNormals(normals, 4);
	geometry->setTextureCoords(uvs, 4);
	geometry->setColors(colors, 4);
	const auto tangents = readUploadedTangents(geometry);
	BOOST_TEST(tangents[3] == 1.f);
	BOOST_TEST(tangents[15] == -1.f);
	geometry->getBuffer()->bind();
	GLint enabled = 0, components = 0, vbo = 0;
	glGetVertexAttribiv(8, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
	glGetVertexAttribiv(8, GL_VERTEX_ATTRIB_ARRAY_SIZE, &components);
	glGetVertexAttribiv(8, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo);
	BOOST_TEST(enabled == GL_TRUE);
	BOOST_TEST(components == 4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint bytes = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bytes);
	// Both shared vertices must split, preserving six independent tangent frames.
	BOOST_TEST(bytes == 6 * 16 * int(sizeof(float)));
	unsigned int uploaded[6] = {};
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uploaded), uploaded);
	BOOST_TEST(uploaded[0] != uploaded[3]);
	BOOST_TEST(uploaded[2] != uploaded[4]);
	float splitColor[4] = {};
	glGetBufferSubData(GL_ARRAY_BUFFER, (uploaded[3] * 16 + 6) * sizeof(float), sizeof(splitColor), splitColor);
	BOOST_TEST(splitColor[0] == 1.f);
	BOOST_TEST(splitColor[1] == 0.f);
	BOOST_TEST(splitColor[3] == 1.f);
	BOOST_TEST(geometry->size() == 6);
}


/**
 * Tests regeneration of uploaded tangents and GPU geometry after source attribute and index edits.
 */
BOOST_AUTO_TEST_CASE(Geometry_regeneratesTangentsAndBuffersAfterEdits)
{
	const float positions[] = { 0,0,0, 1,0,0, 0,1,0 };
	const float normals[] = { 0,0,1, 0,0,1, 0,0,1 };
	const float uvs[] = { 0,0, 1,0, 0,1 };
	auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
	geometry->setVertices(positions, 3);
	geometry->setNormals(normals, 3);
	geometry->setTextureCoords(uvs, 3);
	auto checkTangent = [&](std::array<float, 4> expected)
	{
		const auto uploaded = readUploadedTangents(geometry);
		for (unsigned int i = 0; i < 4; ++i)
			BOOST_TEST(uploaded[i] == expected[i], boost::test_tools::tolerance(0.0001f));
	};
	checkTangent({ 1,0,0,1 });
	const float rotated[] = { 0,0,0, 0,1,0, -1,0,0 };
	geometry->setVertices(rotated, 3);
	checkTangent({ 0,1,0,1 });
	const float tiltedNormals[] = { 0,0.70710678f,0.70710678f, 0,0.70710678f,0.70710678f, 0,0.70710678f,0.70710678f };
	geometry->setNormals(tiltedNormals, 3);
	checkTangent({ 0,0.70710678f,-0.70710678f,1 });
	const float swappedUVs[] = { 0,0, 0,2, 2,0 };
	geometry->setTextureCoords(swappedUVs, 3);
	checkTangent({ -1,0,0,-1 });
	const unsigned int indices[] = { 0,1,2, 0,1,2 };
	geometry->setIndices(indices, 6);
	BOOST_TEST(readUploadedTangents(geometry).at(23) == -1.f);
	BOOST_TEST(geometry->getBuffer()->size() == 6);
	checkTangent({ -1,0,0,-1 });
	geometry->removeNormals();
	BOOST_TEST(!geometry->getBuffer()->getGeometryAttributes().normals);
	BOOST_TEST(!geometry->getBuffer()->getGeometryAttributes().tangents);
	geometry->setNormals(normals, 3);
	checkTangent({ -1,0,0,-1 });
	geometry->removeTextureCoords();
	BOOST_TEST(!geometry->getBuffer()->getGeometryAttributes().uvs);
	BOOST_TEST(!geometry->getBuffer()->getGeometryAttributes().tangents);
}


/**
 * Tests that degenerate and non-finite UV faces retain fallback markers without affecting valid neighbors.
 */
BOOST_AUTO_TEST_CASE(Geometry_invalidUVFacesDoNotAffectValidNeighbors)
{
	const float positions[] = { 0,0,0, 1,0,0, 0,1,0, -1,0,0 };
	const float normals[] = { 0,0,1, 0,0,1, 0,0,1, 0,0,1 };
	const unsigned int indices[] = { 0,1,2, 0,2,3 };
	auto geometry = Geometry::create(PrimitiveType::TRIANGLES);
	geometry->setVertices(positions, 4);
	geometry->setNormals(normals, 4);
	geometry->setIndices(indices, 6);
	for (float invalid : { 0.f, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN() })
	{
		const float uvs[] = { 0,0, 1,0, 0,1, invalid,0 };
		geometry->setTextureCoords(uvs, 4);
		const auto tangents = readUploadedTangents(geometry);
		BOOST_TEST(tangents[0] == 1.f);
		BOOST_TEST(tangents[3] == 1.f);
		for (unsigned int corner = 3; corner < 6; ++corner)
			for (unsigned int component = 0; component < 4; ++component)
				BOOST_TEST(tangents[corner * 4 + component] == 0.f);
	}
}