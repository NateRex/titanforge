#include <graphics/objects/InstancedMesh.h>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>
#include <common/exceptions/IllegalArgumentException.h>

Matrix4 InstanceTransform::matrix() const
{
	Matrix4 result = Matrix4::fromTranslation(position);
	result.multiply(Matrix4::fromRotation(rotation), &result);
	result.multiply(Matrix4::fromScaling(scaling.x, scaling.y, scaling.z), &result);
	return result;
}

InstancedMesh::~InstancedMesh()
{
	if (_buffer)
	{
		delete _buffer;
		_buffer = nullptr;
	}
}

const InstanceTransform& InstancedMesh::getInstance(unsigned int index) const
{
	if (index >= _instances.size()) throw IllegalArgumentException("Instance index is out of range");
	return _instances.at(index);
}

void InstancedMesh::setInstance(unsigned int index, const InstanceTransform& instance)
{
	if (index >= _instances.size()) throw IllegalArgumentException("Instance index is out of range");
	_instances.at(index) = instance;
	_bufferNeedsUpdate = true;
}

void InstancedMesh::setInstances(const std::vector<InstanceTransform>& instances)
{
	_instances = instances;
	_bufferNeedsUpdate = true;
}

void InstancedMesh::addInstance(const InstanceTransform& instance)
{
	_instances.push_back(instance);
	_bufferNeedsUpdate = true;
}

void InstancedMesh::removeInstance(unsigned int index)
{
	if (index >= _instances.size()) throw IllegalArgumentException("Instance index is out of range");
	_instances.erase(_instances.begin() + index);
	_bufferNeedsUpdate = true;
}

void InstancedMesh::clearInstances()
{
	_instances.clear();
	_bufferNeedsUpdate = true;
}

void InstancedMesh::configureDrawItem(DrawItem& item)
{
	item.instanceBuffer = _buffer;
	addMaterialVariant(item, ShaderId::INSTANCED_MESH);
}

void InstancedMesh::traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	// Create buffer if it does not yet exist
	if (!_buffer)
	{
		_buffer = geometry->createInstanceBuffer();
	}

	// Update buffer if needed
	if (_bufferNeedsUpdate)
	{
		std::vector<Matrix4> transforms;
		transforms.reserve(_instances.size());
		for (const InstanceTransform& instance : _instances)
		{
			transforms.push_back(instance.matrix());
		}
		
		_buffer->setTransforms(transforms);
		_bufferNeedsUpdate = false;
	}

	Mesh::traverse(state, parentModel, parentNormal);
}
