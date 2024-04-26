#include "module/context.h"
#include "module/debug.h"
#include "module/input.h"
#include "module/render.h"
#include "module/map.h"
#include "module/ui.h"
#include "res_cache.h"
#include "timestep.h"
#include "random.h"

int SDL_main(int argc, char *argv[])
{
    ecs_world_t *world = ecs_init_w_args(argc, argv);
    ecs_log_set_level(0);

    rngInit();

    LOG_TRACE("IDIOMS (VER. %d.%d.%d DEVELOPMENT)", IDIOMS_VERSION_MAJOR, IDIOMS_VERSION_MINOR, IDIOMS_VERSION_PATCH);

    ECS_IMPORT(world, Context);
    ECS_IMPORT(world, Input);
    ECS_IMPORT(world, Ui);
    ECS_IMPORT(world, Map);
    ECS_IMPORT(world, Render);
    ECS_IMPORT(world, Debug);

    ecs_entity_t e1 = ecs_entity(world, { .name = "test_rect1" });
    ecs_entity_t e2 = ecs_entity(world, { .name = "test_rect2" });

    ecs_entity_t map_canvas = ecs_lookup(world, "map_canvas");

    ecs_set(world, e1, Position,   {0,0});
    ecs_set(world, e1, Dimensions, {1,1});
    ecs_add(world, e1, DebugRect);
    ecs_add_pair(world, e1, RenderedOnCanvas, map_canvas);

    ecs_set(world, e2, Position,   {10,10});
    ecs_add(world, e2, SpriteSource);
    ecs_set(world, e2, TextureHandle, {TEX_CREATURES_01});
    ecs_set(world, e2, SpriteIndex,   {0});
    ecs_add_pair(world, e2, RenderedOnCanvas, map_canvas);

    while (!ecs_should_quit(world))
    {
        ecs_progress(world, getFrametime());
    }

    ecs_fini(world);

    return 0;
}
