#pragma once

typedef struct {
    char *value;
} C_Text;

extern ECS_COMPONENT_DECLARE(C_Text);

void UiImport(ecs_world_t *world);