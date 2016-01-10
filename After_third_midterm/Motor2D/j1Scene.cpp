#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	window =  NULL;
	slider = NULL;
	value_label = NULL;
		/*
	button = NULL;
	button_title = window_title = NULL;
	*/
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
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");
	
	
	//Gui Element
	window = App->gui->AddGuiImage({ 350, 60 }, { 17, 514, 448, 497 }, NULL, this);
	window->interactable = true;
	window->draggable = true;
	window->mask = true;

	/*
	button = App->gui->AddGuiImage({ 110 , 300 }, { 642, 169, 229, 69 }, window, this);
	button->interactable = true;
	button->focusable = true;
	button->draggable = true;
	button->Center(true, false);

	input = App->gui->AddGuiInputBox("Your name", NULL, { 50, 100 }, 315, { 488, 569, 344, 61 }, { -13, -14 }, window, this);
	input->interactable = true;
	input->focusable = true;
	input->draggable = true;
	input->Center(true, false);

	input2 = App->gui->AddGuiInputBox("Your name", NULL, { 50, 100 }, 315, { 488, 569, 344, 61 }, { -13, -14 }, window, this);
	input2->interactable = true;
	input2->focusable = true;
	input2->draggable = true;
	input2->password = true;
	input2->Center(true, true);

	window_title = App->gui->AddGuiLabel("Window", NULL, { 200, 50 }, window, this);

	button_title = App->gui->AddGuiLabel("Button", NULL, { 95, 20 }, button, this);
	*/

	//EXERCISE 1
	slider = App->gui->AddGuiSlider({ 0, 0 }, { 0, 11, 307, 11 }, { 805, 318, 26, 15 }, 297, -2, { -5, 0 }, 1.0f, window, this);
	slider->Center(true, true);
	slider->interactable = true;
	slider->focusable = true;
	//---------

	//EXERCISE 5
	p2SString value_string("Value : %.3f", slider->GetValue());
	value_label = App->gui->AddGuiLabel(value_string.GetString(), NULL, { 0, 50 }, window, this);
	value_label->Center(true, false);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}


	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//Gui
	
	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}


	// Gui ---
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

void j1Scene::OnEvent(GuiElement* element, GUI_Event even)
{	
	/*if (button == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
			element->SetTextureRect({ 411, 169, 229, 69 });
			break;
		case EVENT_MOUSE_LEFTCLICK_UP:
			element->SetTextureRect({ 0, 113, 229, 69 });
			break;
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
			break;
		case EVENT_MOUSE_RIGHTCLICK_UP:
			break;
		case EVENT_MOUSE_ENTER:
		case EVENT_FOCUS_DOWN:
			element->SetTextureRect({ 0, 113, 229, 69 });
			break;
		case EVENT_MOUSE_EXIT:
		case EVENT_FOCUS_UP:
			element->SetTextureRect({ 642, 169, 229, 69 });
			break;
		}
	}*/
	//EXERCISE 4
	if (slider == element)
		switch (even)
	{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
			slider->clicking = true;
			break;
		case EVENT_MOUSE_LEFTCLICK_UP:
			slider->clicking = false;
			break;
		//EXERCISE 5
		case EVENT_SLIDER_CHANGE:
		{
			p2SString value_string("Value : %0.2f", slider->GetValue());
			value_label->SetText(value_string);
		}
		break;
	}

	
	//----
}
