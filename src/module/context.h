#pragma once

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

/*
 * A context is a wrapper around SDL_Window and SDL_Renderer. It is used to
 * initialize and shutdown SDL, manage the window, and to draw stuff to
 * the screen.
 */
typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
} C_Context;

/* 
 * A game configuration is a struct that contains all the configuration
 * options for the game. Later it should be able to be loaded from a
 * config file and edited by the user.
 */
typedef struct GameConf {
    ivec2   window_size;
    uint8_t scaling; 
} GameConf;

/*
 * A canvas is a struct that contains information about the virtual
 * canvas that entities are drawn on.
 */
typedef struct Canvas { 
    uint8_t unit_size;
    bool supports_camera;
} Canvas;

extern ECS_TAG_DECLARE(RenderedOnCanvas);
extern ECS_COMPONENT_DECLARE(C_Context);
extern ECS_COMPONENT_DECLARE(GameConf);
extern ECS_COMPONENT_DECLARE(Canvas);

void ContextImport(ecs_world_t *world);