#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/modes/SurfaceNormalMode.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PointMaterial.h>
#include <math/Vector3.h>

/**
 * Tests item property updates when in surface normal rendering mode
 */
BOOST_AUTO_TEST_CASE(SurfaceNormalRenderMode_apply)
{
    // Test on mesh, containing surface normals
	const BoxGeometryPtr meshGeometry = BoxGeometry::create(1.f, 1.f, 1.f);
	const MeshMaterialPtr meshMaterial = MeshMaterial::create();
	RenderItem meshItem = { meshGeometry.get(), meshMaterial.get() };

	RenderMode::SURFACE_NORMALS->apply(meshItem);
	BOOST_TEST(meshItem.visible);
	BOOST_CHECK(meshItem.shader == ShaderId::NORMALS);
	BOOST_CHECK(meshItem.layer == RenderLayer::OPAQUE);

    // Test on point, which does not contain any surface normals
	const PointGeometryPtr pointGeometry = PointGeometry::create(Vector3::ZERO);
	const PointMaterialPtr pointMaterial = PointMaterial::create();
	RenderItem pointItem = { pointGeometry.get(), pointMaterial.get() };

	RenderMode::SURFACE_NORMALS->apply(pointItem);
    BOOST_TEST(!pointItem.visible);
}
