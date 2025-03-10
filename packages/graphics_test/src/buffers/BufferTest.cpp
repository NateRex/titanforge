#include <boost/test/unit_test.hpp>
#include <graphics/buffers/BufferManager.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector3.h>
#include <graphics/Color.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests construction and destruction of a buffer via the buffer manager
 */
BOOST_AUTO_TEST_CASE(Buffer_basics)
{
	BOOST_REQUIRE_NO_THROW(BufferManager::startBuffer("test-buffer").finish());
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
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
	int indices[] = {
		0, 1, 2
	};

	PPolyface p1(verts, 3, indices, 3);				// no color
	PPolyface p2(verts, 3, indices, 3, colors);		// color
	BufferManager::Builder builder = BufferManager::startBuffer("test-buffer").add(p1);
	BOOST_REQUIRE_THROW(builder.add(p2), IllegalArgumentException);
}