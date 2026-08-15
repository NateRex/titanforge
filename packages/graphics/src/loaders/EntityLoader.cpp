#include <graphics/loaders/EntityLoader.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/materials/Material.h>
#include <graphics/core/EntityGroup.h>
#include <common/Utils.h>
#include <common/exceptions/InstantiationException.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <sstream>
#include <vector>

void applyTransform(Entity& entity, const aiMatrix4x4& transform)
{
	aiVector3D scaling;
	aiVector3D position;
	aiQuaternion rotation;
	transform.Decompose(scaling, rotation, position);

	const aiMatrix3x3 matrix = rotation.GetMatrix();

	entity.setPosition(position.x, position.y, position.z);
	entity.setRotation(
		matrix.a1, matrix.a2, matrix.a3,
		matrix.b1, matrix.b2, matrix.b3,
		matrix.c1, matrix.c2, matrix.c3);
	entity.setScaling(scaling.x, scaling.y, scaling.z);
}

TexturePtr loadTexture(const aiMaterial* aiMat, aiTextureType type, const std::filesystem::path& modelDirectory)
{
	aiString path;
	if (aiMat->GetTextureCount(type) == 0 || aiMat->GetTexture(type, 0, &path) != AI_SUCCESS)
	{
		return nullptr;
	}

	// TODO - Assimp denotes embedded textures with an asterisk. At the time of writing this, TitanForge currently only loads textures from image paths.
	if (path.length > 0 && path.C_Str()[0] == '*')
	{
		return nullptr;
	}

	// Normalize path for loading via TextureLoader
	std::filesystem::path resolvedPath(path.C_Str());
	if (resolvedPath.is_relative()) {
		resolvedPath = modelDirectory / resolvedPath;
	}
	return TextureLoader::load(resolvedPath.lexically_normal().generic_string());
}

MaterialPtr loadMaterial(const aiMaterial* aiMat, const std::filesystem::path& modelDirectory)
{
	MaterialPtr material = Material::create();

	// Color
	aiColor4D color;
	aiReturn result = aiGetMaterialColor(aiMat, AI_MATKEY_BASE_COLOR, &color);
	if (result != AI_SUCCESS)
	{
		result = aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &color);
	}
	if (result == AI_SUCCESS)
	{
		float opacity = color.a;
		aiMat->Get(AI_MATKEY_OPACITY, opacity);
		material->color = Color(color.r, color.g, color.b, std::clamp(opacity, 0.f, 1.f));
	}

	// Reflectivity
	float reflectivity;
	if (aiMat->Get(AI_MATKEY_REFLECTIVITY, reflectivity) == AI_SUCCESS)
	{
		material->reflectivity = std::clamp(reflectivity, 0.f, 1.f);
	}

	// Shine
	float shine;
	if (aiMat->Get(AI_MATKEY_SHININESS, shine) == AI_SUCCESS)
	{
		// TitanForge maps [0, 1] logarithmically ono the exponent range [2, 256]
		material->shine = std::clamp((shine - 2.f) / 254.f, 0.f, 1.f);
	}

	// Texture
	material->texture = loadTexture(aiMat, aiTextureType_BASE_COLOR, modelDirectory);
	if (!material->texture)
	{
		material->texture = loadTexture(aiMat, aiTextureType_DIFFUSE, modelDirectory);
	}

	// Light maps
	material->diffuseMap = loadTexture(aiMat, aiTextureType_LIGHTMAP, modelDirectory);
	material->specularMap = loadTexture(aiMat, aiTextureType_SPECULAR, modelDirectory);
	
	return material;
}

EntityGroupPtr loadEntity(const aiNode* node, const aiScene* scene, const std::vector<MaterialPtr>& materials)
{
	EntityGroupPtr entity = EntityGroup::create();
	applyTransform(*entity, node->mTransformation);

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		MaterialPtr material = materials.at(aiMesh->mMaterialIndex);
	}
}

EntityGroupPtr EntityLoader::load(const std::string& path)
{
	const std::string modelPath = resolvePath(path);
	const std::filesystem::path modelDirectory = std::filesystem::path(path).parent_path();

	// TODO - At the time of writing this, TitanForge does not yet support points and lines
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
	const aiScene* scene = importer.ReadFile(modelPath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals |
		aiProcess_SortByPType);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::ostringstream msg;
		msg << "Failed to load model '" << modelPath << "': " << importer.GetErrorString();
		throw InstantiationException(msg.str());
	}

	std::vector<MaterialPtr> materials;
	materials.reserve(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		materials.push_back(loadMaterial(scene->mMaterials[i], modelDirectory));
	}

	return loadEntity(scene->mRootNode, scene, materials);
}