#include <graphics/buffers/BufferManager.h>
#include <graphics/buffers/Buffer.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <sstream>

// ------------------------------------------------------------------------------------------------------------------
// BufferManager::Builder
// ------------------------------------------------------------------------------------------------------------------

BufferManager::Builder::Builder(const std::string& name) : _name(name)
{

}

BufferManager::Builder& BufferManager::Builder::add(const Mesh& mesh)
{
	if (_vertexData.size() > 0)
	{
		if (_attributes != mesh.getVertexAttributes())
		{
			throw IllegalArgumentException("Meshes of a single buffer must have matching attributes");
		}
	}
	else {
		_attributes = mesh.getVertexAttributes();
	}

	mesh.buffer(_vertexData, _indexData);
	return *this;
}

Buffer* BufferManager::Builder::finish()
{
	Buffer buffer(_name);
	buffer.create(_attributes, _vertexData.data(), _vertexData.size(), _indexData.data(), _indexData.size());
	return BufferManager::addBuffer(buffer);
}


// ------------------------------------------------------------------------------------------------------------------
// BufferManager
// ------------------------------------------------------------------------------------------------------------------

std::mutex BufferManager::_MUTEX;
std::unordered_map<std::string, Buffer> BufferManager::_BUFFERS;

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

Buffer* BufferManager::get(const std::string& name)
{
	auto it = _BUFFERS.find(name);
	if (it == _BUFFERS.end())
	{
		std::ostringstream oss;
		oss << "No buffer found with name: " << name;
		throw IllegalArgumentException(oss.str());
	}

	return &it->second;
}

BufferManager::Builder BufferManager::startBuffer(const std::string& name)
{
	return BufferManager::Builder(name);
}

void BufferManager::destroy(const std::string& name)
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

Buffer* BufferManager::addBuffer(Buffer& buffer)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_BUFFERS.find(buffer.name) != _BUFFERS.end())
	{
		std::ostringstream oss;
		oss << "A buffer with this name already exists: " << buffer.name;
		throw IllegalArgumentException(oss.str());
	}

	auto [it, inserted] = _BUFFERS.emplace(buffer.name, buffer);
	return &it->second;
}