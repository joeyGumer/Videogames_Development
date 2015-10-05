#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum orientation
{
	ORTHOGONAL,
	ISOMETRIC,
	STAGGERED,
};

enum render_order
{
	RIGHT_DOWN,
	RIGHT_UP,
	LEFT_DOWN,
	LEFT_UP,
};
// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct Tileset
{
	//initial attributes
	uint				firstgid;
	p2SString			name;
	uint				tilewidth;
	uint				tileheight;
	uint				spacing;
	uint				margin;
	//for the total tileset texture
	SDL_Texture*		texture;
	uint				texture_width;
	uint				texture_height;

};

// TODO 1: Create a struct needed to hold the information to Map node
struct Map_data
{
	float				version;
	orientation			orientation;
	render_order		renderorder;
	uint				width;
	uint				height;
	uint				tilewidth;
	uint				tileheight;
	uint				nextObjectID;
	p2List<Tileset*>	tilesets;
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

private:
	//load map function
	bool LoadMap();
	//load tileset configuration data to map 
	bool LoadTilesetConfig(pugi::xml_node& tileset, Tileset*);
	//load tileset texture data to map
	bool LoadTilesetImage(pugi::xml_node& tileset, Tileset*);


public:

	// TODO 1: Add your struct for map info as public for now
	Map_data map;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__