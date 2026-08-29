#include <boost/test/unit_test.hpp>
#include <graphics/core/windows/WindowFlags.h>

/**
 * Tests combining and evaluating window flags
 */
BOOST_AUTO_TEST_CASE(WindowFlags_ops)
{
    const WindowFlags resizeAndFS = WindowFlags::RESIZABLE | WindowFlags::FULLSCREEN;
    BOOST_TEST(hasFlag(resizeAndFS, WindowFlags::RESIZABLE));
    BOOST_TEST(hasFlag(resizeAndFS, WindowFlags::FULLSCREEN));
    BOOST_TEST(!hasFlag(resizeAndFS, WindowFlags::ANTI_ALIASING));
    BOOST_TEST(!hasFlag(resizeAndFS, WindowFlags::VSYNC));
    BOOST_TEST(!hasFlag(resizeAndFS, WindowFlags::NONE));

    const WindowFlags all = resizeAndFS | WindowFlags::ANTI_ALIASING | WindowFlags::VSYNC;
    BOOST_TEST(hasFlag(all, WindowFlags::ANTI_ALIASING));
    BOOST_TEST(hasFlag(all, WindowFlags::VSYNC));

    BOOST_TEST(!hasFlag(WindowFlags::NONE, WindowFlags::ANTI_ALIASING));
    BOOST_TEST(!hasFlag(WindowFlags::NONE, WindowFlags::RESIZABLE));
    BOOST_TEST(!hasFlag(WindowFlags::NONE, WindowFlags::FULLSCREEN));
    BOOST_TEST(!hasFlag(WindowFlags::NONE, WindowFlags::VSYNC));
}
