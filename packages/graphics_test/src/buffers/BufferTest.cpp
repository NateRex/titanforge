#include <boost/test/unit_test.hpp>
#include <graphics/buffers/BufferManager.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/objects/Mesh.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <graphics/Color.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Tests the basic constructors and accessors of a buffer
 */
BOOST_AUTO_TEST_CASE(Buffer_basics)
{
	Buffer* buffer = BufferManager::startBuffer("test-buffer").finish();
	BOOST_TEST(buffer->name == "test-buffer");
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
}

/**
 * Tests that meshes added to a buffer must all share the same attributes
 */
BOOST_AUTO_TEST_CASE(Buffer_attributesMustMatch)
{
	// Start builder with no attributes
	float v1[] = {
		-1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};
	Mesh p1(v1, 9, { false, false });
	BufferManager::Builder builder = BufferManager::startBuffer("test-buffer").add(p1);

	// Cannot add mesh containing colors
	float v2[] = {
		-1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f
	};
	Mesh p2(v2, 21, { true, false });
	BOOST_REQUIRE_THROW(builder.add(p2), IllegalArgumentException);

	// Cannot add mesh containing texture coordinates
	float v3[] = {
		-1.f, 0.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.5f, 1.f,
		1.f, 0.f, 0.f, 1.f, 0.f
	};
	Mesh p3(v3, 15, { false, true });
	BOOST_REQUIRE_THROW(builder.add(p3), IllegalArgumentException);
}