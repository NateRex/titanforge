

/**
 * Test fixture capable of executing code before and after all tests
 */
class GlobalTestFixture
{
public:

    /**
     * Code to run before all tests
     */
    GlobalTestFixture();

    /**
     * Code to run after all tests
     */
    ~GlobalTestFixture();
};