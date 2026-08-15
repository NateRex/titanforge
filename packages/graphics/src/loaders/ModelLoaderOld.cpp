#include <graphics/loaders/ModelLoader.h>
#include <graphics/core/EntityGroup.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/objects/Mesh.h>
#include <graphics/textures/TextureLoader.h>
#include <common/Utils.h>
#include <common/exceptions/InstantiationException.h>
#include <math/Matrix3.h>

#include <assimp/Importer.hpp>
#include <assimp/config.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <sstream>
#include <vector>

std::string texturePath(const std::filesystem::path& modelDirectory, const aiString& importedPath)
{
	std::filesystem::path path(importedPath.C_Str());
	if (path.is_relative())
	{
		path = modelDirectory / path;
	}
	return path.lexically_normal().generic_string();
}

TexturePtr loadTexture(const aiMaterial* imported, aiTextureType type,
	const std::filesystem::path& modelDirectory)
{
	aiString path;
	if (imported->GetTextureCount(type) == 0 || imported->GetTexture(type, 0, &path) != AI_SUCCESS)
	{
		return nullptr;
	}

	// Assimp denotes embedded textures with an asterisk-prefixed scene index.
	// TitanForge currently loads textures from image paths only.
	if (path.length > 0 && path.C_Str()[0] == '*')
	{
		return nullptr;
	}

	return TextureLoader::load(texturePath(modelDirectory, path));
}

MaterialPtr convertMaterial(const aiMaterial* imported, const std::filesystem::path& modelDirectory)
{
	BasicMaterialPtr material = BasicMaterial::create();

	aiColor4D color;
	if (aiGetMaterialColor(imported, AI_MATKEY_BASE_COLOR, &color) != AI_SUCCESS)
	{
		aiGetMaterialColor(imported, AI_MATKEY_COLOR_DIFFUSE, &color);
	}
	if (aiGetMaterialColor(imported, AI_MATKEY_BASE_COLOR, &color) == AI_SUCCESS ||
		aiGetMaterialColor(imported, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
	{
		float opacity = color.a;
		imported->Get(AI_MATKEY_OPACITY, opacity);
		material->color = Color(color.r, color.g, color.b, std::clamp(opacity, 0.f, 1.f));
	}

	float reflectivity;
	if (imported->Get(AI_MATKEY_REFLECTIVITY, reflectivity) == AI_SUCCESS)
	{
		material->reflectivity = std::clamp(reflectivity, 0.f, 1.f);
	}

	float shininess;
	if (imported->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
	{
		// TitanForge maps [0, 1] logarithmically onto the exponent range [2, 256].
		material->shine = std::clamp((shininess - 2.f) / 254.f, 0.f, 1.f);
	}

	material->texture = loadTexture(imported, aiTextureType_BASE_COLOR, modelDirectory);
	if (!material->texture)
	{
		material->texture = loadTexture(imported, aiTextureType_DIFFUSE, modelDirectory);
	}
	material->diffuseMap = loadTexture(imported, aiTextureType_LIGHTMAP, modelDirectory);
	material->specularMap = loadTexture(imported, aiTextureType_SPECULAR, modelDirectory);
	return material;
}

BasicMaterialPtr materialForMesh(const MaterialPtr& source, bool useVertexColors)
{
	const BasicMaterialPtr imported = std::static_pointer_cast<BasicMaterial>(source);
	BasicMaterialPtr material = BasicMaterial::create();
	material->color = imported->color;
	material->reflectivity = imported->reflectivity;
	material->shine = imported->shine;
	material->texture = imported->texture;
	material->diffuseMap = imported->diffuseMap;
	material->specularMap = imported->specularMap;
	material->useVertexColors = useVertexColors;
	return material;
}

GeometryPtr convertGeometry(const aiMesh* mesh)
{
	GeometryPtr geometry = Geometry::create();
	std::vector<float> vertices;
	vertices.reserve(mesh->mNumVertices * 3);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
	}
	geometry->setVertices(vertices.data(), mesh->mNumVertices);

	std::vector<unsigned int> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
	}
	geometry->setIndices(indices.data(), static_cast<unsigned int>(indices.size()));

	if (mesh->HasNormals())
	{
		std::vector<float> normals;
		normals.reserve(mesh->mNumVertices * 3);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			normals.insert(normals.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		}
		geometry->setNormals(normals.data(), mesh->mNumVertices);
	}

	if (mesh->HasVertexColors(0))
	{
		std::vector<float> colors;
		colors.reserve(mesh->mNumVertices * 4);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiColor4D& color = mesh->mColors[0][i];
			colors.insert(colors.end(), { color.r, color.g, color.b, color.a });
		}
		geometry->setColors(colors.data(), mesh->mNumVertices);
	}

	if (mesh->HasTextureCoords(0))
	{
		std::vector<float> uvs;
		uvs.reserve(mesh->mNumVertices * 2);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			uvs.push_back(std::clamp(mesh->mTextureCoords[0][i].x, 0.f, 1.f));
			uvs.push_back(std::clamp(mesh->mTextureCoords[0][i].y, 0.f, 1.f));
		}
		geometry->setTextureCoords(uvs.data(), mesh->mNumVertices);
	}

	return geometry;
}

void applyTransform(Entity& entity, const aiMatrix4x4& transform)
{
	aiVector3D scaling;
	aiVector3D position;
	aiQuaternion rotation;
	transform.Decompose(scaling, rotation, position);
	const aiMatrix3x3 matrix = rotation.GetMatrix();

	entity.setPosition(position.x, position.y, position.z);
	entity.setRotation(matrix.a1, matrix.a2, matrix.a3,
		matrix.b1, matrix.b2, matrix.b3,
		matrix.c1, matrix.c2, matrix.c3);
	entity.setScaling(scaling.x, scaling.y, scaling.z);
}

EntityGroupPtr convertNode(const aiNode* node, const aiScene* scene, const MaterialCache& materials)
{
	EntityGroupPtr group = EntityGroup::create();
	applyTransform(*group, node->mTransformation);

	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		const aiMesh* imported = scene->mMeshes[node->mMeshes[i]];
		BasicMaterialPtr material = materialForMesh(
			materials.at(imported->mMaterialIndex), imported->HasVertexColors(0));
		group->add(Mesh::create(convertGeometry(imported), material));
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		group->add(convertNode(node->mChildren[i], scene, materials));
	}
	return group;
}

EntityGroupPtr ModelLoader::load(const std::string& path)
{
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
	const std::string resolvedPath = resolvePath(path);
	const unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals | aiProcess_SortByPType;
	const aiScene* scene = importer.ReadFile(resolvedPath, flags);
	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		std::ostringstream message;
		message << "Failed to load model '" << path << "': " << importer.GetErrorString();
		throw std::runtime_error(message.str());
	}

	const std::filesystem::path modelDirectory = std::filesystem::path(path).parent_path();
	MaterialCache materials;
	materials.reserve(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		materials.push_back(convertMaterial(scene->mMaterials[i], modelDirectory));
	}
	return convertNode(scene->mRootNode, scene, materials);
}
