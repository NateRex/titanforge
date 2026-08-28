#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/DrawState.h>

/**
 * Verify that we can obtain a variant for a given rendering mode
 */
BOOST_AUTO_TEST_CASE(DrawState_getVariant)
{
	DrawItem item;
    item.variants.push_back({ RenderModes::VERTEX_NORMALS, ShaderId::VERTEX_NORMALS, DrawLayer::OPAQUE });
    BOOST_TEST(item.variant(RenderModes::VERTEX_NORMALS) != nullptr);
    BOOST_TEST(item.variant(RenderModes::MATERIAL) == nullptr);
}