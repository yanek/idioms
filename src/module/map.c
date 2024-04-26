#include "map.h"

ECS_COMPONENT_DECLARE(MapData);
ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Dimensions);

void MapImport(ecs_world_t *world) 
{ 
	ECS_MODULE(world, Map);

	ECS_COMPONENT_DEFINE(world, MapData);
	ECS_COMPONENT_DEFINE(world, Position);
	ECS_COMPONENT_DEFINE(world, Dimensions);
}