#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	debug_tex = NULL;

	start.x = start.y = 0;
	goal.x = goal.y = 0;
	click = false;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("iso.tmx");

	debug_tex = App->tex->Load("textures/path.png");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	/*
	//Inputs
	*/

	//Load and Save
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	
	//Camera
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	//Sets debug square
	//Does the Pathfinding with A*
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (!click)
		{
			goal.x = goal.y = 0;
			start.x = start.y = 0;

			int x, y;
			App->input->GetMousePosition(x, y);
			start = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
			click = true;
		}
		else
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			goal = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
			click = false;

			App->pf->ClearLists();
			App->pf->Astar(start, goal);
		}
	}

	
	//--------
	//Paints Map
	App->map->Draw();

	//Places Debug square
	iPoint p = App->map->MapToWorld(start.x, start.y);
	App->render->Blit(debug_tex, p.x, p.y);

	if (!click)
	{
		p = App->map->MapToWorld(goal.x, goal.y);
		App->render->Blit(debug_tex, p.x, p.y);
	}

	//Shows tile position
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//Draws Debug Path
	if (App->pf->path.start)
	{
		p2List_item<PathNode*>* item = App->pf->path.start;

		while (item)
		{
			iPoint pos = App->map->MapToWorld(item->data->pos.x, item->data->pos.y);
			App->render->Blit(debug_tex, pos.x, pos.y);
			item = item->next;
		}
	}

	
	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
