#pragma once
#include <graphics/objects/pointers/InstancedMeshPtr.h>
#include <graphics/objects/Mesh.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <memory>
#include <vector>

class InstanceBuffer;

/**
 * The local transform for a single mesh instance
 * @author Nathaniel Rex
 */
struct InstanceTransform
{
	Vector3 position = Vector3::ZERO;
	Matrix3 rotation = Matrix3::IDENTITY;
	Vector3 scaling = Vector3(1.f, 1.f, 1.f);

	/**
	 * @return The four-dimensional matrix representing the transform consisting of position, scaling, and rotation
	 */
	Matrix4 matrix() const;

};

/**
 * A mesh with instanced rendering support. Use it to efficiently render many objects that share geometry and material
 * but have different transforms, reducing draw calls and improving rendering performance.
 * @author Nathaniel Rex
 */
class InstancedMesh : public Mesh
{
public:
	
	/**
	 * Destructor
	 */
	~InstancedMesh();

	/**
	 * Creates a new instanced mesh from a specified geometry and material. There will initially be zero instances.
	 * @param geometry Geometry
	 * @param material Material
	 */
	static InstancedMeshPtr create(GeometryPtr geometry, MaterialPtr material)
	{
		return std::shared_ptr<InstancedMesh>(new InstancedMesh(geometry, material, {}));
	}

	/**
	 * Creates a new instanced mesh from a specified geometry, material, and a starting set of instances defined by their
	 * individual transforms.
	 * @param geometry Geometry
	 * @param material Material
	 * @param instances The transforms describing each instance
	 */
	static InstancedMeshPtr create(GeometryPtr geometry, MaterialPtr material, const std::vector<InstanceTransform>& instances)
	{
		return std::shared_ptr<InstancedMesh>(new InstancedMesh(geometry, material, instances));
	}

	/**
	 * @return The number of instances
	 */
	unsigned int size() const { return _instances.size(); }

	/**
	 * Obtain the transform for an instance
	 * @param index Index of the instance
	 * @return The instance transform
	 */
	const InstanceTransform& getInstance(unsigned int index) const;

	/**
	 * Update the transform for an instance
	 * @param index Index of the instance to update
	 * @param instance Updated transform for the instance
	 */
	void setInstance(unsigned int index, const InstanceTransform& instance);

	/**
	 * Replaces all instances
	 * @param instances Instance transforms
	 */
	void setInstances(const std::vector<InstanceTransform>& instances);

	/**
	 * Adds a new instance
	 * @param instance Instance transform
	 */
	void addInstance(const InstanceTransform& instance = InstanceTransform());

	/**
	 * Removes an instance
	 * @param index Index of the instance to remove
	 */
	void removeInstance(unsigned int index);

	/**
	 * Removes all instances
	 */
	void clearInstances();

	void configureDrawItem(DrawItem& item) override;

	void traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

	/**
	 * Constructor
	 * @param geometry Geometry
	 * @param material Material
	 * @param instances The instance transforms
	 */
	InstancedMesh(GeometryPtr geometry, MaterialPtr material, const std::vector<InstanceTransform>& instances)
		: Mesh(geometry, material), _instances(instances) {}

private:

	/**
	 * Instance transforms
	 */
	std::vector<InstanceTransform> _instances;

	/**
	 * Instance buffer
	 */
	InstanceBuffer* _buffer = nullptr;

	/**
	 * Boolean flag indicating whether the instance buffer needs to be updated before being accessed
	 */
	bool _bufferNeedsUpdate = true;
};
