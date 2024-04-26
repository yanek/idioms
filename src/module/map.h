#pragma once

#define MAP_SIDE_SIZE 100

enum GridResolution { 
	GRID_RES_WORLD = 16,
	GRID_RES_UI = 8,
};

/*
 * The map component is used to store the map data. The map is a 2D grid of
 * cells, each cell is a byte where 0 is floor and any other number represents
 * a different kind of wall. The map is stored in row-major order, so the first
 * row is stored first, then the second row, etc.
 *
 * The map is stored in a flat array, so we calculate the index of a cell
 * as follows: i = y * row_size + x
 */
typedef struct MapData { 
	uint8_t cells[MAP_SIDE_SIZE * MAP_SIDE_SIZE];
} MapData;

/* 
 * The world position component is used to store the position, in map units,
 * of an entity in the world. A "map unit" is a unit of measurement that 
 * corresponds to 1 * tile_size.
 */
typedef struct Position { 
	int x;
	int y;
} Position;

typedef struct Dimensions { 
    int w;
    int h;
} Dimensions;

extern ECS_COMPONENT_DECLARE(MapData);
extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Dimensions);
extern ECS_TAG_DECLARE(Player);

void MapImport(ecs_world_t *world);