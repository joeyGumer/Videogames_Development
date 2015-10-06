#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 1: Create a struct for the map layer
struct Layer
{
	p2SString			name;
	int					width;
	int					height;
	uint*				data;

	//This returns the tile id of the x,y (in tiles) position at the map that is stored at layer data array.
	//This formula is like this because for each y, we get a full row of tiles done, so to know the integer number of the 
	//tile we multiply y for the width and we add the actua x,  we get the actual tile  (total tiles registered)
	uint Get(int x, int y) const;
};
// TODO 6: Inside struct for the layer, create a short method to get the value of x,y
// ----------------------------------------------------
//done inline outside of the struc, just because we are nubs :v
//this one works
inline uint Layer::Get(int x, int y) const
{
	return data[x + y*width];
}
// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	SDL_Rect RectFind(uint tileID) const
	{
		uint gid = tileID - firstgid;
		int tile_x;
		int tile_y;
		//i'll recomend to write all these formulas on my notebook
		
		if (gid >= num_tiles_width)
		{
			tile_x = gid % num_tiles_width;
			tile_y = (gid - tile_x) / num_tiles_width;
		}
		else
		{
			tile_x = gid;
			tile_y = 0;
		}
		//LOG("tileID = %d, gid = %d, tile x = %d, tile y = %d", tileID, gid, tile_x, tile_y);
		//pixel position
		int x = tile_x*(tile_width + margin) + margin;
		int y = tile_y*(tile_height + spacing) + spacing;

		return{ x, y, tile_width, tile_height};
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List<Layer*>		layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single layer
	bool LoadLayer(pugi::xml_node& node, Layer* layer);


public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__