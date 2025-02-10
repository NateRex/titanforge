#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

/**
 * Main entrypoint for the application, which will run all tests defined using the 'TEST_CASE' definition
 * @param argc The number of arguments
 * @param argv The arguments to the program
 * @return The result, which will be zero if all tests passed, and non-zero if one or more tests fail.
 */
int main(int argc, char** argv)
{
    Catch::Session session;
    return session.run(argc, argv);
}
