#define BOOST_TEST_MODULE GraphicsTest
#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <GLFW/glfw3.h>

/**
 * Test fixture capable of executing code before and after all tests
 */
class GlobalTestFixture
{
public:

    /**
     * Code to run before all tests
     */
    GlobalTestFixture()
    {
        Engine::start(true);
    }

    /**
     * Code to run after all tests
     */
    ~GlobalTestFixture() {
        Engine::stop();
    }
};

BOOST_GLOBAL_FIXTURE(GlobalTestFixture);