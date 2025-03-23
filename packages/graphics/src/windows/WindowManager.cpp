#include <graphics/windows/WindowManager.h>
#include <graphics/windows/Window.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::mutex WindowManager::_MUTEX;
std::unordered_map<std::string, Window> WindowManager::_ALL_WINDOWS;
Window* WindowManager::_CURRENT_WINDOW = nullptr;

void WindowManager::setup()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

}

void WindowManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto& entry : _ALL_WINDOWS)
	{
		entry.second.destroy();
	}

	_ALL_WINDOWS.clear();
}

Window* WindowManager::create(const std::string& name, unsigned int width, unsigned int height)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_ALL_WINDOWS.find(name) != _ALL_WINDOWS.end())
	{
		std::ostringstream oss;
		oss << "A window with this name already exists: " << name;
		throw IllegalArgumentException(oss.str());
	}

	Window win(name);
	win.create(width, height);
	auto [it, inserted] = _ALL_WINDOWS.emplace(name, win);

	return &it->second;
}

Window* WindowManager::get(const std::string& name)
{
	auto it = _ALL_WINDOWS.find(name);
	if (it == _ALL_WINDOWS.end())
	{
		std::ostringstream oss;
		oss << "A window with this name does not exist: " << name;
		throw IllegalArgumentException(oss.str());
	}

	return &it->second;
}

Window* WindowManager::getCurrent()
{
	return _CURRENT_WINDOW;
}

void WindowManager::setCurrent(const std::string& name)
{
	Window* window = get(name);

	std::lock_guard<std::mutex> lock(_MUTEX);

	// Update context
	glfwMakeContextCurrent(window->_glfwWindow);

	// Set viewport dimensions
	int width, height;
	glfwGetWindowSize(window->_glfwWindow, &width, &height);
	glViewport(0, 0, width, height);

	_CURRENT_WINDOW = window;
}

void WindowManager::destroy(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	auto it = _ALL_WINDOWS.find(name);
	if (it == _ALL_WINDOWS.end())
	{
		std::ostringstream oss;
		oss << "A window with this name does not exist: " << name;
		throw IllegalArgumentException(oss.str());
	}

	it->second.destroy();
	_ALL_WINDOWS.erase(name);
}