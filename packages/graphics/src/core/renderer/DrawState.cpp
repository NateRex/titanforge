#include <graphics/core/renderer/DrawState.h>
#include <algorithm>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <common/Utils.h>
#include <glad/glad.h>

const DrawItem::Variant* DrawItem::variant(RenderModes mode) const
{
    const auto it = std::find_if(variants.begin(), variants.end(), [mode](const DrawItem::Variant& candidate) {
        return candidate.mode == mode;
    });

    return it != variants.end() ? &*it : nullptr;
}
void DrawItem::drawGeometry() const
{
	// Draw buffer
	const bool instanced = instanceBuffer != nullptr;
	const GeometryBuffer* buffer = getBuffer();
	const GeometryAttributes& attributes = buffer->getGeometryAttributes();
	const unsigned int primitiveType = toGLPrimitive(attributes.primitiveType);
	const unsigned int instanceCount = instanced ? instanceBuffer->getNumberOfInstances() : 1;
	buffer->bind();
	if (attributes.indices)
	{
		if (instanced)
			glDrawElementsInstanced(primitiveType, buffer->size(), GL_UNSIGNED_INT, 0, instanceCount);
		else
			glDrawElements(primitiveType, buffer->size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		if (instanced)
			glDrawArraysInstanced(primitiveType, 0, buffer->size(), instanceCount);
		else
			glDrawArrays(primitiveType, 0, buffer->size());
	}
}

const GeometryBuffer* DrawItem::getBuffer() const
{
    return instanceBuffer ? static_cast<const GeometryBuffer*>(instanceBuffer) : geometryBuffer;
}
