#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/core/shaders/BasicShader.h>
#include <common/Assertions.h>
#include <glad/glad.h>

ShaderManager::ShaderManager()
{
	_shaders.emplace(MaterialType::BASIC, BasicShader::create());
}

ShaderManager::~ShaderManager()
{
	glUseProgram(0);
	_shaders.clear();
}

ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager _INSTANCE;		// created only once, and will exist until program exit
	return _INSTANCE;
}

ShaderPtr ShaderManager::getShader(MaterialType matType)
{
	ShaderManager& mgr = getInstance();

	assertKeyInMap(mgr._shaders, matType, "Could not find shader for material");
	return mgr._shaders[matType];
}