#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"


#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

/*
---GUI Elements
*/
//Constructors 
GuiElement::GuiElement(iPoint p, GUI_Type t, GuiElement* par = NULL, j1Module* list = NULL): local_pos(p), type(t), parent(par), listener(list), mouseIn(false)
{}

GuiElement::GuiElement(iPoint p, SDL_Rect r, GUI_Type t, GuiElement* par, j1Module* list) : local_pos(p), parent(par), rect(r), type(t), listener(list), mouseIn(false)
{}

GuiLabel::GuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, j1Module* list = NULL) : GuiElement(p, GUI_LABEL, par, list), text(t), font(f)
{
	tex = App->font->Print(text.GetString());
	rect.w = 0, rect.h = 0;
	App->font->CalcSize(text.GetString(), rect.w, rect.h);
	rect = { p.x, p.y, rect.w, rect.h };
}

GuiImage::GuiImage(iPoint p, SDL_Rect r, GuiElement* par, j1Module* list = NULL) : GuiElement(p, r, GUI_IMAGE, par, list)
{}
//-----

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

bool GuiElement::CheckCollision(iPoint p)
{
	bool ret = false;
	if (p.x >  GetScreenPosition().x &&
		p.x < (GetScreenPosition().x + rect.w) &&
		p.y >  GetScreenPosition().y &&
		p.y < (GetScreenPosition().y + rect.h))
	{
		ret = true;
	}
	return ret;
}

//Creators
GuiElement* j1Gui::AddGuiImage(iPoint p, SDL_Rect r, GuiElement* par, j1Module* list)
{
	GuiImage* image = new GuiImage(p, r,par, list);
	return image;
}

GuiElement* j1Gui::AddGuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, j1Module* list)
{
	GuiLabel* label;

	if (f)
		label = new GuiLabel(t, f, p, par, list);
	else
		label = new GuiLabel(t, App->font->default, p, par, list);
	
	return label;
}
//-----

//Draw functions
void GuiImage::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), GetScreenPosition().x - App->render->camera.x, GetScreenPosition().y - App->render->camera.y, &rect);
}

void GuiLabel::Draw()
{
	//SDL_Texture* label = App->font->Print(text.GetString());
	tex = App->font->Print(text.GetString());
	App->render->Blit(tex, GetScreenPosition().x - App->render->camera.x, GetScreenPosition().y - App->render->camera.y, NULL);

	//This is very warro
	App->font->CalcSize(text.GetString(), rect.w, rect.h);
	rect = { GetScreenPosition().x, GetScreenPosition().y, rect.w, rect.h };
}
//

//Update functions
bool GuiImage::Update()
{
	//put the draw functions here
	Draw();
	CheckEvent();
	
	return true;
}

bool GuiLabel::Update()
{
	//put the draw functions here
	Draw();
	CheckEvent();

	return true;
}
//

//GUI_Element general functions

iPoint GuiElement::GetLocalPosition()
{
	return local_pos;
}

iPoint GuiElement::GetScreenPosition()
{
	if (parent)
	{
		iPoint ret;
		ret.x = local_pos.x + parent->GetScreenPosition().x;
		ret.y = local_pos.y + parent->GetScreenPosition().y;
		return ret;
	}

	return local_pos;
}

SDL_Rect GuiElement::GetScreenRect()
{
	return{ GetScreenPosition().x, GetScreenPosition().y, rect.w, rect.y };
}
SDL_Rect GuiElement::GetLocalRect()
{
	return{ local_pos.x, local_pos.y, rect.w, rect.y };
}

bool GuiElement::CheckEvent()
{
	bool collision = CheckCollision(App->input->GetMousePosition());

	if (listener)
	{
		if (collision && !mouseIn)
		{
			mouseIn = true;
			listener->OnEvent(this, EVENT_MOUSE_ENTER);
		}
		else if (!collision && mouseIn)
		{
			mouseIn = false;
			listener->OnEvent(this, EVENT_MOUSE_EXIT);
		}

		if (collision)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_DOWN);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_UP);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_DOWN);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_UP);
		}
	}

	return true;
}

// class Gui ---------------------------------------------------

