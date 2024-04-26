#include "time.h"
#include <SDL.h>

float getFrametime()
{
    static uint64_t last_ticks;
    static uint64_t now_ticks;

    last_ticks = now_ticks;
    now_ticks = SDL_GetPerformanceCounter();

    return (float) (now_ticks - last_ticks) /
           (float) SDL_GetPerformanceFrequency();
}

float getFps()
{
    return 1.0f / getFrametime();
}