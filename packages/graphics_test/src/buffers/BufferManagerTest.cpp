#include <boost/test/unit_test.hpp>
#include <graphics/buffers/BufferManager.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Tests that a buffer can be constructed, bound (and drawn), unbound, and destroyed via the manager
 */
BOOST_AUTO_TEST_CASE(BufferManager_binding)
{
	BufferManager::startBuffer("buffer1").finish();
	BufferManager::startBuffer("buffer2").finish();

	// At this point, buffer2 should be bound
	GLuint binding1;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*) &binding1);

	// Bind and draw buffer1
	BufferManager::draw("buffer1");

	// Verify buffer2 no longer bound
	GLuint binding2;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*) &binding2);
	BOOST_TEST(binding1 != binding2);

	// Destroy both buffers
	BufferManager::destroy("buffer1");
	BufferManager::destroy("buffer2");
	
	// Verify no buffer is bound
	GLuint binding3;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*) &binding3);
	BOOST_TEST(binding3 == 0);
}

/**
 * Tests that an exception is thrown if we try to draw a buffer that does not exist
 */
BOOST_AUTO_TEST_CASE(BufferManager_bindFailure)
{
	BOOST_REQUIRE_THROW(BufferManager::draw("does-not-exist"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to destroy a buffer that does not exist
 */
BOOST_AUTO_TEST_CASE(BufferManager_destroyFailure)
{
	BOOST_REQUIRE_THROW(BufferManager::destroy("does-not-exist"), IllegalArgumentException);

	BufferManager::startBuffer("test-buffer").finish();
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
	BOOST_REQUIRE_THROW(BufferManager::destroy("test-buffer"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to create more than one buffer with the same name
 */
BOOST_AUTO_TEST_CASE(BufferManager_duplicateNames)
{
	BufferManager::startBuffer("test-buffer").finish();
	BOOST_REQUIRE_THROW(BufferManager::startBuffer("test-buffer").finish(), IllegalArgumentException);

	BufferManager::destroy("test-buffer");
}