#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <graphics/core/EntityGroup.h>
#include <graphics/loaders/ModelLoader.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/objects/Mesh.h>
#include <common/exceptions/InstantiationException.h>
#include <math/Matrix3.h>
#include <math/Vector3.h>
#include <vector>

namespace
{
	GeometryPtr processMesh(const aiMesh* importedMesh)
	{
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> textureCoords;
		std::vector<unsigned int> indices;

		vertices.reserve(importedMesh->mNumVertices * 3);
		normals.reserve(importedMesh->mNumVertices * 3);
		textureCoords.reserve(importedMesh->mNumVertices * 2);

		for (unsigned int i = 0; i < importedMesh->mNumVertices; ++i)
		{
			const aiVector3D& vertex = importedMesh->mVertices[i];
			vertices.insert(vertices.end(), { vertex.x, vertex.y, vertex.z });

			if (importedMesh->HasNormals())
			{
				const aiVector3D& normal = importedMesh->mNormals[i];
				normals.insert(normals.end(), { normal.x, normal.y, normal.z });
			}

			if (importedMesh->HasTextureCoords(0))
			{
				const aiVector3D& uv = importedMesh->mTextureCoords[0][i];
				textureCoords.insert(textureCoords.end(), { uv.x, uv.y });
			}
		}

		indices.reserve(importedMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < importedMesh->mNumFaces; ++i)
		{
			const aiFace& face = importedMesh->mFaces[i];
			indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
		}

		GeometryPtr geometry = Geometry::create();
		geometry->setVertices(vertices.data(), importedMesh->mNumVertices);
		geometry->setIndices(indices.data(), static_cast<unsigned int>(indices.size()));

		if (!normals.empty())
		{
			geometry->setNormals(normals.data(), importedMesh->mNumVertices);
		}

		if (!textureCoords.empty())
		{
			geometry->setTextureCoords(textureCoords.data(), importedMesh->mNumVertices);
		}

		return geometry;
	}

	EntityGroupPtr processNode(const aiNode* node, const aiScene* scene, const std::vector<GeometryPtr>& geometries)
	{
		EntityGroupPtr group = EntityGroup::create();

		aiVector3D scaling;
		aiVector3D position;
		aiQuaternion rotation;
		node->mTransformation.Decompose(scaling, rotation, position);
		const aiMatrix3x3 rotationMatrix = rotation.GetMatrix();

		group->setPosition(position.x, position.y, position.z);
		group->setRotation(
			rotationMatrix.a1, rotationMatrix.a2, rotationMatrix.a3,
			rotationMatrix.b1, rotationMatrix.b2, rotationMatrix.b3,
			rotationMatrix.c1, rotationMatrix.c2, rotationMatrix.c3);
		group->setScaling(scaling.x, scaling.y, scaling.z);

		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			group->add(Mesh::create(geometries[node->mMeshes[i]], BasicMaterial::create()));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			group->add(processNode(node->mChildren[i], scene, geometries));
		}

		return group;
	}
}

EntityGroupPtr ModelLoader::load(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

	if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 || !scene->mRootNode)
	{
		throw InstantiationException("Failed to load model '" + path + "': " + importer.GetErrorString());
	}

	std::vector<GeometryPtr> geometries;
	geometries.reserve(scene->mNumMeshes);

	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		geometries.push_back(processMesh(scene->mMeshes[meshIndex]));
	}

	return processNode(scene->mRootNode, scene, geometries);
}
