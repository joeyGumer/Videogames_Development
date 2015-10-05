#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)
	for (p2List_item<Tileset*>* tileset = map.tilesets.start; tileset != NULL; tileset = tileset->next)
	{
		SDL_Rect r = { 0, 0, tileset->data->texture_width, tileset->data->texture_height };

		App->render->Blit(tileset->data->texture, 0, 0, &r);
	}

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	
	// Remove all tilesets
	//unload tileset texturesç
	//don't know if this is needed
	LOG("Unloading the tilesets' textures before removing them");
	for (p2List_item<Tileset*>* tileset = map.tilesets.start; tileset != NULL; tileset = tileset->next)
		App->tex->UnLoad(tileset->data->texture);
	
	map.tilesets.clear();

	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char* buf;
	int size = App->fs->Load(tmp.GetString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		ret = LoadMap();
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset_node;
	for (tileset_node = map_file.child("map").child("tileset"); tileset_node && ret; tileset_node = tileset_node.next_sibling("tileset"))
	{
		Tileset* set = new Tileset();

		ret = LoadTilesetConfig(tileset_node, set);

		if (ret)
			ret = LoadTilesetImage(tileset_node, set);
		
		if (ret)
			map.tilesets.add(set);
	}

	// TODO 5: LOG all the data loaded
	// iterate all tilesets and LOG everything
	//this shows the results of the loading
	if (ret)
	{
		LOG("succsesfuly parsed map XML file : %s", file_name);
		LOG("width: %d height: %d", map.width, map.height);
		LOG("tile_width: %d tile_height: %d", map.tilewidth, map.tileheight);
		LOG("Tileset ---");
		LOG("name: %s firstgid: %d", map.tilesets[0]->name.GetString(), map.tilesets[0]->firstgid);
		LOG("tile_width: %d tile_height: %d", map.tilesets[0]->tilewidth, map.tilesets[0]->tileheight);
		LOG("spacing: %d margin: %d", map.tilesets[0]->spacing, map.tilesets[0]->margin);
		LOG("Image ---");
		LOG("width: %d height: %d", map.tilesets[0]->texture_width, map.tilesets[0]->texture_height);
	}

	map_loaded = ret;

	return ret;
}
//function that loads the map
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node  map_node = map_file.child("map");
	
	if (map_node == NULL)
	{
		LOG("Error to parsing map node in xml file: didn't found map tag");
		ret = false;
	}
	else
	{
		map.version = map_node.attribute("version").as_float();

		p2SString orientation(map_node.attribute("orientation").as_string());
		if (orientation == "orthogonal")
			map.orientation = ORTHOGONAL;
		else if (orientation == "isometric")
			map.orientation = ISOMETRIC;
		else if (orientation == "staggered")
			map.orientation = STAGGERED;

		p2SString renderorder(map_node.attribute("renderorder").as_string());
		if (renderorder == "right-down")
			map.renderorder = RIGHT_DOWN;
		else if (renderorder == "right-up")
			map.renderorder = RIGHT_UP;
		else if (renderorder == "left-down")
			map.renderorder = LEFT_DOWN;
		else if (renderorder == "left-up")
			map.renderorder = LEFT_UP;

		map.width			= map_node.attribute("width").as_int();
		map.height			= map_node.attribute("height").as_int();
		map.tilewidth		= map_node.attribute("tilewidth").as_int();
		map.tileheight		= map_node.attribute("tileheight").as_int();

		map.nextObjectID	= map_node.attribute("nextobjectid").as_int();
	}

	return ret;
}

bool j1Map::LoadTilesetConfig(pugi::xml_node& tileset_node, Tileset* tileset)
{
	bool ret = true;
	tileset->name.create(tileset_node.attribute("name").as_string());
	
	tileset->firstgid	= tileset_node.attribute("firstgid").as_int();
	tileset->tilewidth	= tileset_node.attribute("tilewidth").as_int();
	tileset->tileheight	= tileset_node.attribute("tileheight").as_int();
	tileset->spacing	= tileset_node.attribute("spacing").as_int();
	tileset->margin		= tileset_node.attribute("margin").as_int();

	return true;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, Tileset* tileset)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	p2SString directory("%s%s",folder.GetString(),image.attribute("source").as_string());
	tileset->texture = App->tex->Load(directory.GetString());
	
	tileset->texture_width = image.attribute("width").as_int();
	tileset->texture_height = image.attribute("height").as_int();

	return ret;
}