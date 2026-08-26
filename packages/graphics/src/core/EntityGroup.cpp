#include <graphics/core/EntityGroup.h>

EntityGroupPtr EntityGroup::create()
{
	return std::shared_ptr<EntityGroup>(new EntityGroup());
}