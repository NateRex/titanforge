#pragma once
#include <graphics/objects/pointers/PointsPtr.h>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/objects/Renderable.h>
#include <vector>
#include <initializer_list>

class Vector3;

/**
 * Represents a collection of primitive points or a point cloud.
 * @author Nathaniel Rex
 */
class Points : public Renderable
{
public:

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
    static PointsPtr create(PointGeometryPtr geometry, MaterialPtr material)
    {
        return std::shared_ptr<Points>(new Points(geometry, material));
    }

protected:

    /**
     * Constructor
     * @param geometry Point geometry
     * @param material Material
     */
    Points(PointGeometryPtr geometry, MaterialPtr material) : Renderable(geometry, material) {}
};