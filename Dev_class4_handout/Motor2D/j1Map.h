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
	STAGGERED
};

enum render_order
{
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_UP,
	RIGHT_DOWN
};
// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

struct tileSet
{
	int firstgid;

	p2SString name;

	unsigned int tilewidth;
	unsigned int tileheight;


};

// TODO 1: Create a struct needed to hold the information to Map node
struct map_node
{
	orientation orientation;
	render_order render_order;

	int width;
	int height;

	int tilewidth;
	int tileheight;

	int nextobjectid;
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

public:

	// TODO 1: Add your struct for map info as public for now

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__