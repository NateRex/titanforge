#include <boost/test/unit_test.hpp>
#include <graphics/objects/Lines.h>
#include <graphics/geometry/LineGeometry.h>
#include <graphics/materials/LineMaterial.h>

/**
 * Tests the basic construction of lines
 */
BOOST_AUTO_TEST_CASE(Lines_basics)
{
    // From geometry
	LineGeometryPtr geometry = LineGeometry::createSegments(&Vector3::ZERO, 1);
	MaterialPtr material = LineMaterial::create();
	LinesPtr object = Lines::create(geometry, material);
	BOOST_TEST(object->geometry == geometry);
	BOOST_TEST(object->material == material);

    // Segments from vector
    std::vector<Vector3> points = {
        Vector3::XHAT,
        Vector3::YHAT
    };
    object = Lines::createSegments(points, material);
    BOOST_TEST(object != nullptr);

    // Segments from intializer list
    object = Lines::createSegments({
        Vector3::XHAT,
        Vector3::YHAT
    }, material);
    BOOST_TEST(object != nullptr);
    
    // Polyline from vector
    object = Lines::createPolyline(points, material);
    BOOST_TEST(object != nullptr);

    // Polyline from initializer list
    object = Lines::createPolyline({
        Vector3::XHAT,
        Vector3::YHAT
    }, material);
    BOOST_TEST(object != nullptr);
}