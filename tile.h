#ifndef TILE_H
#define TILE_H

typedef enum
{
	WALL,
	FIRE,
	POOP,
	EMPTY,
	DOOR
} TileType;

typedef union
{
	struct
	{
		int hitsLeft;
		int damage;

	} fire;
	struct
	{
		int hitsLeft;
	} poop;

} TileData;

typedef struct
{
	Rectangle rect;
	Color color;
	bool isSolid;
	TileType type;
	TileData data;
} Tile;

#endif
