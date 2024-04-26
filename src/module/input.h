#pragma once

#include <SDL_scancode.h>

/*
 * The keyboard state component is used to store the current and previous state
 * of the keyboard. It is used by the input system to determine if a key is
 * pressed, released, down or repeating.
 */
typedef struct {
    bool current_state[SDL_NUM_SCANCODES];
    bool previous_state[SDL_NUM_SCANCODES];
} C_KeyboardState;

extern ECS_COMPONENT_DECLARE(C_KeyboardState);

void InputImport(ecs_world_t *world);

/*
 * Below are a number of helper functions that can be used to query the
 * keyboard state and check if a key is pressed, released, down or repeating.
 */
bool inputIsDown(const C_KeyboardState *kbs, uint8_t scancode);
bool inputIsReleased(const C_KeyboardState *kbs, uint8_t scancode);
bool inputIsPressed(const C_KeyboardState *kbs, uint8_t scancode);
bool inputIsRepeating(const C_KeyboardState *kbs, uint8_t scancode);
