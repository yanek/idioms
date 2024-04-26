#include "res_cache.h"
#include <SDL.h>
#include <SDL_image.h>

#define RESOURCE_DIRECTORY_ROOT "res/"

static texture_t *textureCache[16];
static texture_t *loadTexture(SDL_Renderer *renderer, const char *path);

texture_t *getTexture(uint8_t id)
{
    return textureCache[id];
}

void loadAllTextures(C_Context *context)
{
    LOG_TRACE("Loading resources:");
    SDL_Renderer *renderer = context->renderer;

    textureCache[TEX_CREATURES_01] = loadTexture(renderer, "s_creatures_01.png");
    textureCache[TEX_FONT_01]      = loadTexture(renderer, "f_terminal.png");
    textureCache[TEX_FONT_02]      = loadTexture(renderer, "f_dbyte.png");
}

texture_t *loadTexture(SDL_Renderer *renderer, const char *path)
{
    size_t bufsize = strlen(RESOURCE_DIRECTORY_ROOT) + strlen(path) + 1;
    char *fullpath = malloc(bufsize);
    strcpy_s(fullpath, bufsize, RESOURCE_DIRECTORY_ROOT);
    strcat_s(fullpath, bufsize, path);
    LOG_TRACE("\t(TEXTURE):%s...", fullpath);

    SDL_Surface *surf = IMG_Load(fullpath);
    if (surf == NULL)
    {
        LOG_ERROR("Failed to load texture: %s", SDL_GetError());
        return NULL;
    }

    texture_t *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == NULL)
    {
        LOG_ERROR("Failed to create texture: %s", SDL_GetError());
        return NULL;
    }

    return texture;
}
