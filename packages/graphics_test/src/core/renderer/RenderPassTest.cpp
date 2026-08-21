#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/RenderPass.h>

/**
 * Tests combining and evaluating clear flags
 */
BOOST_AUTO_TEST_CASE(RenderPass_clearFlags)
{
    const ClearFlags colorAndDepth = ClearFlags::COLOR | ClearFlags::DEPTH;
    BOOST_TEST(hasFlag(colorAndDepth, ClearFlags::COLOR));
    BOOST_TEST(hasFlag(colorAndDepth, ClearFlags::DEPTH));
    BOOST_TEST(!hasFlag(colorAndDepth, ClearFlags::STENCIL));
    BOOST_TEST(!hasFlag(colorAndDepth, ClearFlags::NONE));

    const ClearFlags all = colorAndDepth | ClearFlags::STENCIL;
    BOOST_TEST(hasFlag(all, ClearFlags::COLOR));
    BOOST_TEST(hasFlag(all, ClearFlags::DEPTH));
    BOOST_TEST(hasFlag(all, ClearFlags::STENCIL));

    BOOST_TEST(!hasFlag(ClearFlags::NONE, ClearFlags::COLOR));
    BOOST_TEST(!hasFlag(ClearFlags::NONE, ClearFlags::DEPTH));
    BOOST_TEST(!hasFlag(ClearFlags::NONE, ClearFlags::STENCIL));
}
