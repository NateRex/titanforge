#include <boost/test/unit_test.hpp>
#include <graphics/entities/EntityGroup.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic construction of an entity group
 */
BOOST_AUTO_TEST_CASE(EntityGroup_basics)
{
	EntityGroupPtr group = EntityGroup::create();
	BOOST_TEST(group->type == EntityType::GROUP);
}