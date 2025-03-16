#include <graphics/window/WindowManager.h>
#include <graphics/window/Window.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::mutex WindowManager::_MUTEX;
std::map<std::string, Window> WindowManager::_OPEN_WINDOWS;

void WindowManager::setup()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	// Currently, this method does nothing. However, this may change in the future.
}

void WindowManager::refresh()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto it = _OPEN_WINDOWS.begin(); it != _OPEN_WINDOWS.end(); ) {
		if (glfwWindowShouldClose(it->second._glfwWindow))
		{
			it->second.destroy();
			it = _OPEN_WINDOWS.erase(it);
		}
		else {
			it++;
		}
	}
}

void WindowManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto& entry : _OPEN_WINDOWS)
	{
		entry.second.destroy();
	}

	_OPEN_WINDOWS.clear();
}

Window* WindowManager::create(const std::string& name, unsigned int width, unsigned int height)
{
	std::unique_lock<std::mutex> lock(_MUTEX);

	if (_OPEN_WINDOWS.find(name) != _OPEN_WINDOWS.end())
	{
		std::ostringstream oss;
		oss << "A window with this name already exists: " << name;
		throw IllegalArgumentException(oss.str());
	}

	Window win(name);
	win.create(width, height);
	auto [it, inserted] = _OPEN_WINDOWS.emplace(name, win);

	lock.unlock();

	if (_OPEN_WINDOWS.size() == 1)
	{
		// This is the only window, make it the current rendering target
		WindowManager::setCurrent(name);
	}

	return &it->second;
}

Window* WindowManager::get(const std::string& name)
{
	auto it = _OPEN_WINDOWS.find(name);
	if (it == _OPEN_WINDOWS.end())
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
