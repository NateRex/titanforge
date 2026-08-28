#include <boost/test/unit_test.hpp>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <glad/glad.h>

/**
 * Tests uploading per-instance transformation matrices.
 */
BOOST_AUTO_TEST_CASE(InstanceBuffer_setTransforms)
{
	float vertices[] = {
		-1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};

	InstanceBuffer buffer(GeometryAttributes(), vertices, 9, nullptr, 0);
	BOOST_TEST(buffer.getNumberOfInstances() == 0);

	std::vector<Matrix4> transforms = {
		Matrix4::IDENTITY,
		Matrix4::fromTranslation(Vector3(1.f, 2.f, 3.f))
	};
	BOOST_REQUIRE_NO_THROW(buffer.setTransforms(transforms));
	BOOST_TEST(buffer.getNumberOfInstances() == 2);

	GLint bufferSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	BOOST_TEST(bufferSize == static_cast<GLint>(transforms.size() * 16 * sizeof(float)));
}
