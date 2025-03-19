#include <boost/test/unit_test.hpp>
#include <graphics/buffers/BufferManager.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <graphics/Color.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Tests construction and destruction of a buffer via the buffer manager
 */
BOOST_AUTO_TEST_CASE(Buffer_basics)
{
	Buffer* buffer = BufferManager::startBuffer("test-buffer").finish();
	BOOST_TEST(buffer->name == "test-buffer");
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
}

/**
 * Tests that a buffer can be constructed, bound (and drawn), unbound, and destroyed via the manager
 */
BOOST_AUTO_TEST_CASE(BufferManager_binding)
{
	Buffer* buffer1 = BufferManager::startBuffer("buffer1").finish();
	Buffer* buffer2 = BufferManager::startBuffer("buffer2").finish();

	// At this point, buffer2 should be bound
	GLuint binding1;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&binding1);

	// Draw buffer1
	buffer1->draw();

	// Verify buffer2 no longer bound
	GLuint binding2;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&binding2);
	BOOST_TEST(binding1 != binding2);

	// Destroy both buffers
	BufferManager::destroy("buffer1");
	BufferManager::destroy("buffer2");

	// Verify no buffer is bound
	GLuint binding3;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&binding3);
	BOOST_TEST(binding3 == 0);
}

/**
 * Tests that primitives added to a buffer must all share the same attributes
 */
BOOST_AUTO_TEST_CASE(Buffer_attributesMustMatch)
{
	Vector3 verts[] = {
		Vector3(-1, 0, 0),
		Vector3(0, 1, 0),
		Vector3(1, 0, 0)
	};
	Color colors[] = {
		Color::fromFloats(1, 0, 0, 1),
		Color::fromFloats(0, 1, 0, 1),
		Color::fromFloats(0, 0, 1, 1)
	};
	Vector2 texCoords[] = {
		Vector2(0, 0),
		Vector2(0.5, 1),
		Vector2(1, 0)
	};
	int indices[] = {
		0, 1, 2
	};

	// Start builder with no attributes
	PPolyface p1(verts, 3, indices, 3);
	BufferManager::Builder builder = BufferManager::startBuffer("test-buffer").add(p1);

	// Cannot add primitive containing colors
	PPolyface p2(verts, 3, indices, 3, colors);
	BOOST_REQUIRE_THROW(builder.add(p2), IllegalArgumentException);

	// Cannot add primitive containing texture coordinates
	PPolyface p3(verts, 3, indices, 3, nullptr, texCoords);
	BOOST_REQUIRE_THROW(builder.add(p3), IllegalArgumentException);
}