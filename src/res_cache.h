#pragma once

#include "module/context.h"

typedef struct SDL_Texture texture_t;

/*
 * A ResourceID is a unique identifier for a resource, typically an asset.
 * It is used to reference a resource preloaded in a resource cache,
 * like a texture, a sound, a font, etc.
 */
enum ResourceID
{
    TEX_CREATURES_01,
    TEX_FONT_01,
    TEX_FONT_02,
};

/*
 * Retrieves a texture from the resource cache and returns a
 * pointer to it. If the texture does not exist or hasn't
 * been preloaded yet, NULL is returned.
 */
texture_t *getTexture(uint8_t id);

/*
 * Initializes the resource cache, and loads all the resources
 * into memory.
 *
 * TODO: This could return a ResourceCache struct of some sort,
 * which would prevent the need for a global variable.
 */
void loadAllTextures(C_Context *context);