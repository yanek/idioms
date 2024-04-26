#include "context.h"
#include "../res_cache.h"
#include <SDL.h>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

#define WINDOW_FLAGS   (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)

ECS_TAG_DECLARE(RenderedOnCanvas);
ECS_COMPONENT_DECLARE(C_Context);
ECS_COMPONENT_DECLARE(GameConf);
ECS_COMPONENT_DECLARE(Canvas);

static void Init(ecs_iter_t *it)
{
    C_Context *ctx = ecs_field(it, C_Context, 1);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        LOG_FATAL("Failed to initialize SDL: %s", SDL_GetError());
    }

    SDL_Window *window = SDL_CreateWindow(
        "Idioms",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_FLAGS
    );

    ctx->window = window;
    LOG_TRACE("Window initialized (%dx%d)", WINDOW_WIDTH, WINDOW_HEIGHT);

    ctx->renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    SDL_RendererInfo rendererInfo;
    SDL_GetRenderDriverInfo(0, &rendererInfo);

    LOG_TRACE("Renderer initialized (using %s+%s)", SDL_GetCurrentVideoDriver(), rendererInfo.name);

    loadAllTextures(ctx); // TODO: make this a proper singleton component, too
}

static void ProcessEvents(ecs_iter_t *it)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) { ecs_quit(it->world); }
}

static void Destroy(ecs_iter_t *it)
{
    C_Context *ctx = ecs_field(it, C_Context, 1);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
}

void ContextImport(ecs_world_t *world)
{
    ECS_MODULE(world, Context);

    ECS_TAG_DEFINE(world, RenderedOnCanvas);
    ECS_COMPONENT_DEFINE(world, C_Context);
    ECS_COMPONENT_DEFINE(world, GameConf);
    ECS_COMPONENT_DEFINE(world, Canvas);

    ECS_SYSTEM(world,   Init,          EcsOnStart,  C_Context($));
    ECS_SYSTEM(world,   ProcessEvents, EcsOnUpdate, C_Context($));
    ECS_OBSERVER(world, Destroy,       EcsUnSet,    C_Context($));

    ecs_singleton_add(world, C_Context);
    ecs_singleton_set(world, GameConf, {{1280, 720}, 2});

    ecs_entity_t mapCanvas = ecs_new_id(world);
    ecs_set_name(world, mapCanvas, "map_canvas");
    ecs_set(world, mapCanvas, Canvas, {16, true});
    
    ecs_entity_t uiCanvas = ecs_new_id(world);
    ecs_set_name(world, uiCanvas, "ui_canvas");
    ecs_set(world, uiCanvas, Canvas, {8, false});
}
