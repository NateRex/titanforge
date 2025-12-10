#include <graphics/core/EntityGroup.h>

EntityGroup::EntityGroup() : Entity(EntityType::GROUP)
{

}

EntityGroupPtr EntityGroup::create()
{
	return std::shared_ptr<EntityGroup>(new EntityGroup());
}