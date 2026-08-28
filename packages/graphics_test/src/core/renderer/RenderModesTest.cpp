#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/RenderModes.h>

/**
 * Tests combining and evaluating render modes
 */
BOOST_AUTO_TEST_CASE(RenderMode_modes)
{
    const RenderModes materialOnly = RenderModes::MATERIAL;
    BOOST_TEST(hasFlag(materialOnly, RenderModes::MATERIAL));
    BOOST_TEST(!hasFlag(materialOnly, RenderModes::VERTEX_NORMALS));

    const RenderModes all = materialOnly | RenderModes::VERTEX_NORMALS;
    BOOST_TEST(hasFlag(all, RenderModes::MATERIAL));
    BOOST_TEST(hasFlag(all, RenderModes::VERTEX_NORMALS));
}