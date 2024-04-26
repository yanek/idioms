#include "debug.h"
#include "render.h"
#include "map.h"
#include "context.h"
#include "ui.h"
#include "../res_cache.h"
#include "../timestep.h"
#include <stdio.h>
#include <SDL_render.h>

ECS_TAG_DECLARE(DebugRect);	
ECS_TAG_DECLARE(FrametimeCounter);

static void DrawDebugRectangles(ecs_iter_t *it)
{
    C_Context *context = ecs_field(it, C_Context, 1);
    const Position *pos = ecs_field(it, Position, 2);
    const Dimensions *dim = ecs_field(it, Dimensions, 3);
    const Camera *cam = ecs_field(it, Camera, 4);
    const GameConf *conf = ecs_field(it, GameConf, 5);

    const RgbaColor color = {200, 100, 120, 255};

    for (int i = 0; i < it->count; i++)
    {
        const Canvas *canvas = getEntityCanvas(it->world, it->entities[i]);
        if (!canvas) continue;

        int xoffset, yoffset;
        getCameraOffset(canvas, cam, &xoffset, &yoffset);

        int x = (pos[i].x + xoffset) * canvas->unit_size * conf->scaling;
        int y = (pos[i].y + yoffset) * canvas->unit_size * conf->scaling;
        int w = dim[i].w * canvas->unit_size * conf->scaling;
        int h = dim[i].h * canvas->unit_size * conf->scaling;

        SDL_Rect sdlRect = {x, y, w, h};
        SDL_SetRenderDrawColor(context->renderer, color[0], color[1], color[2], 255);
        SDL_RenderDrawRect(context->renderer, &sdlRect);
    }
}

static void UpdateFrametimeCounter(ecs_iter_t *it)
{
    C_Text *text = ecs_field(it, C_Text, 1);

    for (int i = 0; i < it->count; i++)
    {
        char buffer[64];
        sprintf(buffer, "FRAMETIME: %.2fms", getFrametime() * 1000);
        strcpy_s(text[i].value, sizeof(buffer), buffer);
    }
}

void DebugImport(ecs_world_t *world)
{
    ECS_MODULE(world, Debug);

    ECS_TAG_DEFINE(world, DebugRect);
    ECS_TAG_DEFINE(world, FrametimeCounter);

    ECS_SYSTEM(world, UpdateFrametimeCounter, EcsOnUpdate, ui.C_Text,            FrametimeCounter);
    ECS_SYSTEM(world, DrawDebugRectangles,    EcsOnStore,  context.C_Context($), [in] map.Position, [in] map.Dimensions, [in] render.Camera($), [in] context.GameConf($), DebugRect);

    ecs_set_interval(world, ecs_id(UpdateFrametimeCounter), 0.2f); // Limit tickrate to 0.2Hz

    ecs_entity_t fcnt_e = ecs_entity(world, { .name = "frametime_counter" });
    ecs_add_pair(world, fcnt_e, RenderedOnCanvas, ecs_lookup(world, "ui_canvas"));
    ecs_add(world, fcnt_e, C_Text);
    ecs_set(world, fcnt_e, Position,        {0, 0});
    ecs_set(world, fcnt_e, BackgroundColor, {100, 150, 110, 255});
    ecs_set(world, fcnt_e, TextureHandle,   {TEX_FONT_01});
    ecs_add(world, fcnt_e, FrametimeCounter);
}
