#include <graphics/loaders/ModelLoader.h>
#include <graphics/loaders/TextureLoader.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/core/EntityGroup.h>
#include <graphics/objects/Mesh.h>
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

MeshMaterialPtr loadMaterial(const aiMaterial* aiMat, const std::filesystem::path& modelDirectory)
{
	MeshMaterialPtr material = MeshMaterial::create();

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

GeometryPtr loadGeometry(const aiMesh* aiMesh)
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> colors;
	std::vector<float> uvs;
	vertices.reserve(aiMesh->mNumVertices * 3);
	normals.reserve(aiMesh->mNumVertices * 3);
	colors.reserve(aiMesh->mNumVertices * 4);
	uvs.reserve(aiMesh->mNumVertices * 2);

	const bool hasNormals = aiMesh->HasNormals();
	const bool hasColors = aiMesh->HasVertexColors(0);
	const bool hasUVs = aiMesh->HasTextureCoords(0);

	// Iterate over vertices
	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
		const aiVector3D& vertex = aiMesh->mVertices[i];
		vertices.insert(vertices.end(), { vertex.x, vertex.y, vertex.z });

		if (hasNormals)
		{
			const aiVector3D& normal = aiMesh->mNormals[i];
			normals.insert(normals.end(), { normal.x, normal.y, normal.z });
		}
		if (hasColors)
		{
			const aiColor4D& color = aiMesh->mColors[0][i];
			colors.insert(colors.end(), { color.r, color.g, color.b, color.a });
		}
		if (hasUVs)
		{
			const aiVector3D& uv = aiMesh->mTextureCoords[0][i];
			uvs.push_back(std::clamp(uv.x, 0.f, 1.f));
			uvs.push_back(std::clamp(uv.y, 0.f, 1.f));
		}
	}

	// Iterate over indices
	std::vector<unsigned int> indices;
	indices.reserve(aiMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		const aiFace& face = aiMesh->mFaces[i];
		indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
	}

	// Construct geometry
	GeometryPtr geometry = Geometry::create();
	geometry->setVertices(vertices.data(), aiMesh->mNumVertices);
	geometry->setIndices(indices.data(), indices.size());
	if (hasNormals)
	{
		geometry->setNormals(normals.data(), aiMesh->mNumVertices);
	}
	if (hasColors)
	{
		geometry->setColors(colors.data(), aiMesh->mNumVertices);
	}
	if (hasUVs)
	{
		geometry->setTextureCoords(uvs.data(), aiMesh->mNumVertices);
	}

	return geometry;
}

EntityGroupPtr loadGroup(const aiNode* node, const aiScene* scene, const std::vector<MeshMaterialPtr>& materials)
{
	EntityGroupPtr group = EntityGroup::create();
	applyTransform(*group, node->mTransformation);

	// Handle meshes at this node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

		GeometryPtr geometry = loadGeometry(aiMesh);
		MeshMaterialPtr material = materials.at(aiMesh->mMaterialIndex);
		material->useVertexColors = aiMesh->HasVertexColors(0);

		group->add(Mesh::create(geometry, material));
	}

	// Handle child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		group->add(loadGroup(node->mChildren[i], scene, materials));
	}

	return group;
}

EntityPtr ModelLoader::load(const std::string& path)
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

	std::vector<MeshMaterialPtr> materials;
	materials.reserve(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		materials.push_back(loadMaterial(scene->mMaterials[i], modelDirectory));
	}

	return loadGroup(scene->mRootNode, scene, materials);
}