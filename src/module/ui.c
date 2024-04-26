#include "ui.h"

ECS_COMPONENT_DECLARE(C_Text);

void UiImport(ecs_world_t *world)
{
    ECS_MODULE(world, Ui);

    ECS_COMPONENT_DEFINE(world, C_Text);
}