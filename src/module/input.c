#include "input.h"
#include <SDL_keyboard.h>

ECS_COMPONENT_DECLARE(C_KeyboardState);

static void UpdateKeyboardState(ecs_iter_t *it)
{
    C_KeyboardState *kbs = ecs_field(it, C_KeyboardState, 1);
    const uint8_t *state = SDL_GetKeyboardState(NULL);

    for (int j = 0; j < SDL_NUM_SCANCODES; j++)
    {
        kbs->previous_state[j] = kbs->current_state[j];
        kbs->current_state[j] = state[j];
    }
}

static void HandleQuickExit(ecs_iter_t *it)
{
    const C_KeyboardState *kbs = ecs_field(it, C_KeyboardState, 1);

    if (inputIsReleased(kbs, SDL_SCANCODE_ESCAPE))
    {
        LOG_TRACE("Escape pressed, exiting");
        ecs_quit(it->world);
    }
}

void InputImport(ecs_world_t *world)
{
    ECS_MODULE(world, Input);

    ECS_COMPONENT_DEFINE(world, C_KeyboardState);

    ECS_SYSTEM(world, UpdateKeyboardState, EcsOnUpdate, [inout] C_KeyboardState($));
    ECS_SYSTEM(world, HandleQuickExit,     EcsOnUpdate, [in] C_KeyboardState($));

    ecs_singleton_set(world, C_KeyboardState, {{0}, {0}});
}

/*
 * Helper functions
 */

bool inputIsDown(const C_KeyboardState *kbs, uint8_t scancode)
{
    return kbs->current_state[scancode];
}

bool inputIsReleased(const C_KeyboardState *kbs, uint8_t scancode)
{
    return !kbs->current_state[scancode] && kbs->previous_state[scancode];
}

bool inputIsPressed(const C_KeyboardState *kbs, uint8_t scancode)
{
    return kbs->current_state[scancode] && !kbs->previous_state[scancode];
}

bool inputIsRepeating(const C_KeyboardState *kbs, uint8_t scancode)
{
    // TODO: Find an elegant way to support repeating keys. Or not?
    // Currently it's not really hooking into the OS's repeating key feature.
    // At least, introduce a delay between the "down" state and the
    // "repeating" state.
    return kbs->current_state[scancode] && kbs->previous_state[scancode];
}