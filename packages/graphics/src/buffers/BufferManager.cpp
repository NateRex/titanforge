#include <graphics/buffers/BufferManager.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/primitives/IPrimitive.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <sstream>

// ------------------------------------------------------------------------------------------------------------------
// BufferManager::Builder
// ------------------------------------------------------------------------------------------------------------------

BufferManager::Builder::Builder(const std::string& name) : _name(name)
{

}

void BufferManager::Builder::add(const IPrimitive& primitive)
{
	primitive.buffer(_data);
}

void BufferManager::Builder::finish()
{
	Buffer buffer(_name);
	buffer.create(_data.data(), _data.size());
	BufferManager::addBuffer(buffer);
}


// ------------------------------------------------------------------------------------------------------------------
// BufferManager
// ------------------------------------------------------------------------------------------------------------------

std::mutex BufferManager::_MUTEX;
std::map<std::string, Buffer> BufferManager::_BUFFERS;

void BufferManager::setup()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	// Currently, this method does nothing. However, this may change in the future.
}

void BufferManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto& entry : _BUFFERS)
	{
		entry.second.destroy();
	}

	_BUFFERS.clear();
}

void BufferManager::addBuffer(Buffer& buffer)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_BUFFERS.find(buffer._name) != _BUFFERS.end())
	{
		std::ostringstream oss;
		oss << "A buffer with this name already exists: " << buffer._name;
		throw IllegalArgumentException(oss.str());
	}

	_BUFFERS.emplace(buffer._name, buffer);
}

void BufferManager::bindBuffer(const std::string& name)
{
	auto it = _BUFFERS.find(name);
	if (it == _BUFFERS.end())
	{
		std::ostringstream oss;
		oss << "No buffer found with name: " << name;
		throw IllegalArgumentException(oss.str());
	}

	glBindVertexArray(it->second._vaoId);
}

void BufferManager::removeBuffer(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	auto it = _BUFFERS.find(name);
	if (it == _BUFFERS.end())
	{
		std::ostringstream oss;
		oss << "No buffer found with name: " << name;
		throw IllegalArgumentException(oss.str());
	}

	it->second.destroy();
	_BUFFERS.erase(name);
}