#pragma once
#include <graphics/core/Entity.h>
#include <graphics/materials/pointers/PostProcessMaterialPtr.h>
#include <graphics/objects/pointers/PostProcessingPtr.h>

/**
 * A scene object that applies a full-screen material after the scene has been drawn. If multiple
 * post-processing objects are added to a scene, they will be applied in the order they are found during scene traversal.
 * @author Nathaniel Rex
 */
class PostProcessing : public Entity
{
public:
	
	/**
	 * Material containing the post-processing effect settings.
	 */
	PostProcessMaterialPtr material;

	/**
	 * Creates a post-processing scene object.
	 * @param material Material containing the effect settings
	 * @return The new post-processing object
	 */
	static PostProcessingPtr create(PostProcessMaterialPtr material);

	void traverse(RenderState& state, const RenderPass& pass, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

	/**
	 * @copydoc Entity::updatePosition(float, float, float)
	 * @throws UnsupportedOperationException When called, as changing position of post-processing is not supported.
	 */
	void updatePosition(float x, float y, float z) override;

	/**
	 * @copydoc Entity::updateRotation(float, float, float, float, float, float, float, float, float)
	 * @throws UnsupportedOperationException When called, as changing rotation of post-processing is not supported.
	 */
	void updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) override;

	/**
	 * @copydoc Entity::updateScaling(float, float, float)
	 * @throws UnsupportedOperationException When called, as scaling of post-processing is not supported.
	 */
	void updateScaling(float x, float y, float z) override;

private:
	
	/**
	 * Constructor
	 * @param material Material containing the effect settings
	 */
	PostProcessing(PostProcessMaterialPtr material);
};
