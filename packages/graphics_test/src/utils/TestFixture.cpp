#include <graphics_test/utils/TestFixture.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>

TestFixture::TestFixture()
{
	setup();
}

TestFixture::~TestFixture()
{
	teardown();
}

void TestFixture::setup()
{
	Engine::start();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
}

void TestFixture::teardown()
{
	Engine::stop();
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
}