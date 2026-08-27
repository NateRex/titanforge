#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/PointShader.h>
#include <graphics/core/shaders/LineShader.h>
#include <graphics/core/shaders/MeshShader.h>
#include <graphics/core/shaders/WireframeShader.h>
#include <graphics/core/shaders/SkyboxShader.h>
#include <graphics/core/shaders/PostProcessShader.h>
#include <graphics/core/shaders/NormalsShader.h>
#include <common/Assertions.h>
#include <glad/glad.h>

std::unique_ptr<ShaderManager> ShaderManager::_INSTANCE = nullptr;

ShaderManager::ShaderManager()
{
	_shaders.emplace(ShaderId::POINT, PointShader::create());
	_shaders.emplace(ShaderId::LINE, LineShader::create());
	_shaders.emplace(ShaderId::MESH, MeshShader::create());
	_shaders.emplace(ShaderId::WIREFRAME, WireframeShader::create());
	_shaders.emplace(ShaderId::SKYBOX, SkyboxShader::create());
	_shaders.emplace(ShaderId::POST_PROCESS, PostProcessShader::create());
	_shaders.emplace(ShaderId::NORMALS, NormalsShader::create());
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

ShaderPtr ShaderManager::getShader(ShaderId id)
{
	ShaderManager* mgr = getInstance();

	assertKeyInMap(mgr->_shaders, id, "Could not find shader");
	return mgr->_shaders[id];
}

void ShaderManager::reset()
{
	if (_INSTANCE)
	{
		_INSTANCE.reset();
	}
}
