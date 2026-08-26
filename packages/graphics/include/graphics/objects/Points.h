#pragma once
#include <graphics/objects/pointers/PointsPtr.h>
#include <graphics/geometry/pointers/PointGeometryPtr.h>
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/core/Entity.h>
#include <vector>
#include <initializer_list>

class Vector3;

/**
 * Represents a collection of primitive points or a point cloud.
 * @author Nathaniel Rex
 */
class Points : public Entity
{
public:

    /**
     * Geometry composed of a collection of independent points defined in world-space
     */
    PointGeometryPtr geometry;

	/**
	 * Material
	 */
	MaterialPtr material;

    /**
     * Constructs a new points object
     * @param point A single point defined in world-space coordinates
     * @param material Material
     * @return The new points instance
     */
    static PointsPtr create(const Vector3& point, MaterialPtr material);

    /**
     * Constructs a new points object
     * @param points A set of points defined in world-space coordinates
     * @param material Material
     * @return The new points instance
     */
    static PointsPtr create(const std::vector<Vector3>& points, MaterialPtr material);

    /**
     * Constructs a new points object
     * @param points A set of points defined in world-space coordinates
     * @param material Material
     * @return The new points instance
     */
    static PointsPtr create(std::initializer_list<Vector3> points, MaterialPtr material);

    /**
     * Constructs a new points object
     * @param geometry Point geometry
     * @param material Material
     * @return The new points instance
     */
    static PointsPtr create(PointGeometryPtr geometry, MaterialPtr material);

    void traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal) override;

protected:

    
};