#include <graphics/objects/Renderable.h>
#include <graphics/core/renderer/RenderState.h>

void Renderable::traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
    // Compute local-to-world matrices
    const Matrix4 modelTransform = parentModel.multiply(getLocalMatrix());
	const Matrix3 normalTransform = parentNormal.multiply(getLocalNormalMatrix());

    // Represent this object as a new render item on the state
	RenderItem item;
	item.geometry = geometry.get();
	item.material = material.get();
	item.modelTransform = modelTransform;
	item.normalTransform = normalTransform;
	state.items.push_back(item);

    // Traverse all children
	for (const EntityPtr& child : _children)
	{
		child->traverse(state, modelTransform, normalTransform);
	}
}