#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/core/shaders/MeshShader.h>
#include <graphics/core/shaders/PostProcessShader.h>
#include <common/Assertions.h>
#include <glad/glad.h>

std::unique_ptr<ShaderManager> ShaderManager::_INSTANCE = nullptr;

ShaderManager::ShaderManager()
{
	_shaders.emplace(MaterialType::MESH, MeshShader::create());
	_shaders.emplace(MaterialType::POST_PROCESS, PostProcessShader::create());
}

ShaderManager::~ShaderManager()
{
	glUseProgram(0);
	_shaders.clear();
}

ShaderManager* ShaderManager::getInstance()
{
	if (!_INSTANCE)
	{
		_INSTANCE = std::unique_ptr<ShaderManager>(new ShaderManager());
	}

	return _INSTANCE.get();
}

ShaderPtr ShaderManager::getShader(MaterialType matType)
{
	ShaderManager* mgr = getInstance();

	assertKeyInMap(mgr->_shaders, matType, "Could not find shader for material");
	return mgr->_shaders[matType];
}

void ShaderManager::reset()
{
	if (_INSTANCE)
	{
		_INSTANCE.reset();
	}
}
