#include "render.h"
#include "input.h"
#include "context.h"
#include "ui.h"
#include "map.h"
#include "../res_cache.h"
#include <SDL_render.h>

#define FONT_CHAR_PER_ROW 16
#define FONT_CHAR_PER_COL 16

ECS_COMPONENT_DECLARE(TextureHandle);
ECS_COMPONENT_DECLARE(SpriteSource);
ECS_COMPONENT_DECLARE(SpriteIndex);
ECS_COMPONENT_DECLARE(BackgroundColor);
ECS_COMPONENT_DECLARE(Camera);

// This system is called before the store is processed, so it is called before
// any rendering happens. It is used to clear the screen.
static void Begin(ecs_iter_t *it)
{
    SDL_Renderer *renderer = ecs_field(it, C_Context, 1)->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

// This system is called after the store is processed, so it is called after
// all rendering happens. It is used to present the rendered frame to the user.
static void End(ecs_iter_t *it)
{
    SDL_Renderer *renderer = ecs_field(it, C_Context, 1)->renderer;
    SDL_RenderPresent(renderer);
}


static void DrawSprites(ecs_iter_t *it)
{
    C_Context *context = ecs_field(it, C_Context, 1);
    const Position *pos = ecs_field(it, Position, 2);
    const SpriteSource *src = ecs_field(it, SpriteSource, 3);
    const TextureHandle *tex = ecs_field(it, TextureHandle, 4);
    const Camera *cam = ecs_field(it, Camera, 5);
    const GameConf *conf = ecs_field(it, GameConf, 6);

    for (int i = 0; i < it->count; i++)
    {
        const Canvas* canvas = getEntityCanvas(it->world, it->entities[i]);
        if (!canvas) continue;

        int xoffset, yoffset;
        getCameraOffset(canvas, cam, &xoffset, &yoffset);

        int x = (pos[i].x + xoffset) * canvas->unit_size * conf->scaling;
        int y = (pos[i].y + yoffset) * canvas->unit_size * conf->scaling;
        int wh = canvas->unit_size * conf->scaling;

        SDL_Rect sdlSrc = {src[i].x, src[i].y, src[i].w, src[i].h};
        SDL_Rect sdlDst = {x, y, wh, wh};
        SDL_Texture *texture = getTexture(tex[i].id);
        SDL_RenderCopy(context->renderer, texture, &sdlSrc, &sdlDst);
    }
}

static void DrawText(ecs_iter_t *it)
{
    C_Context *context = ecs_field(it, C_Context, 1);
    const Position *pos = ecs_field(it, Position, 2);
    const C_Text *text = ecs_field(it, C_Text, 3);
    const TextureHandle *tex = ecs_field(it, TextureHandle, 4);
    const BackgroundColor *bg = ecs_field(it, BackgroundColor, 5);
    const GameConf *conf = ecs_field(it, GameConf, 6);

    for (int i = 0; i < it->count; i++)
    {
        SDL_Texture *texture = getTexture(tex[i].id);
        int twidth, theight;
        SDL_QueryTexture(texture, NULL, NULL, &twidth, &theight);

        int cwidth = twidth / FONT_CHAR_PER_ROW;
        int cheight = theight / FONT_CHAR_PER_COL;
        int xoffset = 0;

        char c;
        for (int j = 0; (c = text[i].value[j]) != '\0'; j++)
        {
            const Canvas* canvas = getEntityCanvas(it->world, it->entities[i]);
            if (!canvas) continue;

            int x = (pos[i].x * canvas->unit_size + xoffset) * conf->scaling;
            int y = pos[i].y * canvas->unit_size * conf->scaling;
            int w = cwidth * conf->scaling;
            int h = cheight * conf->scaling;

            SDL_Rect sdl_dst = {x, y, w,h};
            SDL_Rect sdl_src = {(c % FONT_CHAR_PER_ROW) * cwidth, (c / FONT_CHAR_PER_COL) * cheight, cwidth, cheight};

            if (bg)
            {
                SDL_SetRenderDrawColor(context->renderer, bg->value[0], bg->value[1], bg->value[2], bg->value[3]);
                SDL_RenderFillRect(context->renderer, &sdl_dst);
            }

            SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);
            SDL_RenderCopy(context->renderer, texture, &sdl_src, &sdl_dst);
            xoffset += cwidth;
        }
    }
}

static void MoveCamera(ecs_iter_t* it)
{
    Camera *cam = ecs_field(it, Camera, 1);
    const C_KeyboardState *kbs = ecs_field(it, C_KeyboardState, 2);

    if (inputIsPressed(kbs, SDL_SCANCODE_UP))    cam->yoffset++;
    if (inputIsPressed(kbs, SDL_SCANCODE_DOWN))  cam->yoffset--;
    if (inputIsPressed(kbs, SDL_SCANCODE_LEFT))  cam->xoffset++;
    if (inputIsPressed(kbs, SDL_SCANCODE_RIGHT)) cam->xoffset--;
}

 // Calculate the source rect for sprites based on the texture and frame index.
 // For performances reasons, this should be called from an observer watching
 // for SpriteIndex changes rather than a standard system.
static void CalcSpriteSourceRect(ecs_iter_t *it)
{
    const SpriteIndex *sprite = ecs_field(it, SpriteIndex, 1);
    const TextureHandle *tex = ecs_field(it, TextureHandle, 2);
    SpriteSource *src = ecs_field(it, SpriteSource, 3);

    for (int i = 0; i < it->count; i++)
    {
        LOG_TRACE("Calculating source rect for entity %d", it->entities[i]);

        uint8_t frame_idx = sprite[i].index;
        SDL_Texture *texture = getTexture(tex[i].id);
        int twidth, theight;
        SDL_QueryTexture(texture, NULL, NULL, &twidth, &theight);

        src[i].x = (frame_idx * FRAME_SIZE) % twidth;
        src[i].y = (frame_idx * FRAME_SIZE) / theight;
        src[i].w = FRAME_SIZE;
        src[i].h = FRAME_SIZE;
    }
}

void RenderImport(ecs_world_t *world)
{
    ECS_MODULE(world, Render);

    ECS_TAG(world, EcsPostStore);
    ecs_add_pair(world, EcsPostStore, EcsDependsOn, EcsOnStore);

    ECS_COMPONENT_DEFINE(world, TextureHandle);
    ECS_COMPONENT_DEFINE(world, SpriteSource);
    ECS_COMPONENT_DEFINE(world, SpriteIndex);
    ECS_COMPONENT_DEFINE(world, BackgroundColor);
    ECS_COMPONENT_DEFINE(world, Camera);

    ECS_SYSTEM(world,   Begin,                  EcsPreStore,  context.C_Context($));
    ECS_SYSTEM(world,   End,                    EcsPostStore, context.C_Context($));
    ECS_SYSTEM(world,   DrawSprites,            EcsOnStore,   context.C_Context($), [in] map.Position,              [in] SpriteSource,     [in] TextureHandle, [in] Camera($),        [in] GameConf($));
    ECS_SYSTEM(world,   DrawText,               EcsOnStore,   context.C_Context($), [in] map.Position,              [in] ui.C_Text,        [in] TextureHandle, [in] ?BackgroundColor, [in] GameConf($));
    ECS_SYSTEM(world,   MoveCamera,             EcsOnUpdate,  [out] Camera($),      [in] input.C_KeyboardState($));
    ECS_OBSERVER(world, CalcSpriteSourceRect,   EcsOnSet,     [in] SpriteIndex,   [in] TextureHandle,           [out] SpriteSource);

    ecs_singleton_set(world, Camera, {5, 5});
}

/*
 * Helper functions
 */

const Canvas* getEntityCanvas(ecs_world_t* world, ecs_entity_t entity)
{
    if (!ecs_has_pair(world, entity, RenderedOnCanvas, EcsWildcard))
    {
        LOG_ERROR("Entity '%s' (%d) is not assigned to a canvas and will not be rendered", ecs_get_name(world, entity), entity);
        return NULL;
    }

    ecs_entity_t canvas_e = ecs_get_target(world, entity, RenderedOnCanvas, 0);
    const Canvas* canvas = ecs_get(world, canvas_e, Canvas);
    return canvas;
}

void getCameraOffset(const Canvas* canvas, const Camera* cam, int* xoffset, int* yoffset)
{
    if (canvas->supports_camera)
    {
        *xoffset = cam->xoffset;
        *yoffset = cam->yoffset;
    } else {
        *xoffset = 0;
        *yoffset = 0;
    }
}