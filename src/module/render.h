#pragma once

#define FRAME_SIZE 16
/*
 * A texture handle is a unique identifier for a texture. It is used to
 * reference a SDL_Texture preloaded in a resource cache.
 */
typedef struct {
    uint8_t id;
} TextureHandle;

/*
 * A source rect is a rectangle that defines the area of a texture to draw.
 * It is useful for sprite sheets, where each sprite has its own source rect
 * in a big texture atlas.
 */
typedef struct {
    int x;
    int y;
    int w;
    int h;
} SpriteSource;

/*
 * A sprite index is a number that represents the index of a sprite in a
 * sprite sheet. It is used to calculate the source rect for a sprite.
 */
typedef struct {
    uint8_t index;
} SpriteIndex;

/*
 * A background color is an RGBA color that is used to draw behind
 * some renderable entity (currently works for Text only).
 */
typedef struct {
    RgbaColor value;
} BackgroundColor;

/* 
 * A camera is basically just an offset that is applied to all renderable
 * entities in the world space.
 */
typedef struct {
    int xoffset;
    int yoffset;
} Camera;

extern ECS_COMPONENT_DECLARE(TextureHandle);
extern ECS_COMPONENT_DECLARE(SpriteSource);
extern ECS_COMPONENT_DECLARE(SpriteIndex);
extern ECS_COMPONENT_DECLARE(BackgroundColor);
extern ECS_COMPONENT_DECLARE(Camera);

void RenderImport(ecs_world_t *world);

typedef struct Canvas Canvas;
const Canvas*getEntityCanvas(ecs_world_t* world, ecs_entity_t entity);
void getCameraOffset(const Canvas* canvas, const Camera* cam, int* xoffset, int* yoffset);
