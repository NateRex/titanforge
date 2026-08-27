#include <graphics/objects/Renderable.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/core/renderer/RenderPass.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>

void Renderable::traverse(RenderState& state, const RenderPass& pass, const Matrix4& parentModel, const Matrix3& parentNormal)
{
    // Compute local-to-world matrices
    const Matrix4 modelTransform = parentModel.multiply(getLocalMatrix());
	const Matrix3 normalTransform = parentNormal.multiply(getLocalNormalMatrix());

	if (pass.mode == RenderMode::MATERIAL)
	{
		// Produce item for default material rendering
		RenderItem item;
		item.geometry = geometry.get();
		item.material = material.get();
		item.shader = material->shaderId();
		item.modelTransform = modelTransform;
		item.normalTransform = normalTransform;

		if (material->isTransparent())
		{
			item.layer = RenderLayer::TRANSPARENT;
		}
		else if (material->isBackground())
		{
			item.layer = RenderLayer::BACKGROUND;
		}

		state.items.push_back(item);
	}
	else if (pass.mode == RenderMode::VERTEX_NORMALS && geometry->getAttributes().normals)
	{
		// Produce item for rendering vertex normals
		RenderItem item;
		item.geometry = geometry.get();
		item.material = material.get();
		item.shader = ShaderId::VERTEX_NORMALS;
		item.layer = RenderLayer::OPAQUE;
		item.modelTransform = modelTransform;
		item.normalTransform = normalTransform;

		state.items.push_back(item);
	}
    

    // Traverse all children
	for (const EntityPtr& child : _children)
	{
		child->traverse(state, pass, modelTransform, normalTransform);
	}
}