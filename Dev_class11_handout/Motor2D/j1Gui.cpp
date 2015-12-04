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
//May i change this in the future for the rect
GuiElement::GuiElement(iPoint p, GUI_Type t, GuiElement* par = NULL, j1Module* list = NULL): type(t), parent(par), listener(list), mouseIn(false)
{
	local_rect = { p.x, p.y, tex_rect.w, tex_rect.h };
}

GuiElement::GuiElement(iPoint p, SDL_Rect r, GUI_Type t, GuiElement* par, j1Module* list) : parent(par), tex_rect(r), type(t), listener(list), mouseIn(false)
{
	local_rect = { p.x, p.y, tex_rect.w, tex_rect.h };
}

GuiLabel::GuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, j1Module* list = NULL) : GuiElement(p, GUI_LABEL, par, list), text(t), font(f)
{
	//Have to polish the texture sistem in the label
	tex = App->font->Print(text.GetString());
	tex_rect = { 0, 0, 0, 0 };
	App->font->CalcSize(text.GetString(), tex_rect.w, tex_rect.h);
	SetLocalRect({ p.x, p.y, tex_rect.w, tex_rect.h });
}

GuiImage::GuiImage(iPoint p, SDL_Rect r, GuiElement* par, j1Module* list = NULL) : GuiElement(p, r, GUI_IMAGE, par, list)
{}

//I'm doing and especific constructor, have to change this
GuiInputBox::GuiInputBox(p2SString t, _TTF_Font* f, iPoint p, SDL_Rect r, GuiElement* par, j1Module* list) : GuiElement(p, r, GUI_INPUTBOX, par, list), text(t, f, { 20, 20 }, this), image({ 0, 0 }, r, this)
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
		p.x < (GetScreenPosition().x + local_rect.w) &&
		p.y >  GetScreenPosition().y &&
		p.y < (GetScreenPosition().y + local_rect.h))
	{
		ret = true;
	}
	return ret;
}

//Creators
GuiImage* j1Gui::AddGuiImage(iPoint p, SDL_Rect r, GuiElement* par, j1Module* list)
{
	GuiImage* image = new GuiImage(p, r,par, list);
	return image;
}

GuiLabel* j1Gui::AddGuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, j1Module* list)
{
	GuiLabel* label;

	if (f)
		label = new GuiLabel(t, f, p, par, list);
	else
		label = new GuiLabel(t, App->font->default, p, par, list);
	
	return label;
}

GuiInputBox* j1Gui::AddGuiInputBox(p2SString t ,_TTF_Font* f, iPoint p, SDL_Rect r, GuiElement* par, j1Module* list)
{
	GuiInputBox* input = new GuiInputBox(t,f, p, r, par, list);

	return input;
}
//-----

//Draw functions
void GuiImage::Draw()
{
	App->render->Blit(App->gui->GetAtlas(),
					  GetScreenPosition().x - App->render->camera.x, 
					  GetScreenPosition().y - App->render->camera.y, 
					  &tex_rect);
}

void GuiLabel::Draw()
{
	
	tex = App->font->Print(text.GetString());
	App->render->Blit(tex, GetScreenPosition().x - App->render->camera.x, GetScreenPosition().y - App->render->camera.y, NULL);

	//TODO: something happens in this line, if it's activated it moves the labels, and also, i have to delete de texture when changed, ask rick
	//This is very warro
	//App->font->CalcSize(text.GetString(), tex_rect.w, tex_rect.h);
	//SetLocalRect({ GetScreenPosition().x, GetScreenPosition().y, tex_rect.w, tex_rect.h });
}

void GuiInputBox::Draw()
{
	image.Draw();
	text.Draw();
}
//

//Update functions
bool GuiImage::Update()
{
	//put the draw functions here
	CheckEvent();
	Draw();
	
	return true;
}

bool GuiLabel::Update()
{
	//put the draw functions here
	CheckEvent();
	Draw();

	return true;
}

bool GuiInputBox::Update()
{
	Draw();

	return true;
}
//

//GUI_Element general functions

iPoint GuiElement::GetLocalPosition()
{
	return{ local_rect.x, local_rect.y };
}

iPoint GuiElement::GetScreenPosition()
{
	if (parent)
	{
		iPoint ret;
		ret.x = local_rect.x + parent->GetScreenPosition().x;
		ret.y = local_rect.y + parent->GetScreenPosition().y;
		return ret;
	}

	return{ local_rect.x, local_rect.y };
}

SDL_Rect GuiElement::GetScreenRect()
{
	return{ GetScreenPosition().x, GetScreenPosition().y, local_rect.w, local_rect.h };
}
SDL_Rect GuiElement::GetLocalRect()
{
	return local_rect;
}
//maybe with x and y
void GuiElement::SetLocalPosition(iPoint p)
{
	local_rect.x = p.x;
	local_rect.y = p.y;
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
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
				listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_REPEAT);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
				listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_DOWN);
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
				listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_UP);
		}
	}

	return true;
}

//maybe i can do this with each of the elements?
void GuiElement::DrawDebug()
{
	SDL_Rect rect = GetScreenRect();
	rect.x -= App->render->camera.x;
	rect.y -= App->render->camera.y;

	App->render->DrawLine(rect.x, rect.y, rect.x + rect.w, rect.y, 255, 0, 0);
	App->render->DrawLine(rect.x, rect.y, rect.x , rect.y + rect.h, 255, 0, 0);
	App->render->DrawLine(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, 255, 0, 0);
	App->render->DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, 255, 0, 0);

}

//There should be a better way...
//i'll do it tomorrow...
//it's not optimal at all checking the list each frame

GuiElement* j1Gui::FindSelectedElement(p2List<GuiElement*> list)
{
	//CheckCollision(App->input->GetMousePosition());

	p2List_item<GuiElement*>* item = list.end;

	for (; item; item = item->prev)
	{
		if (item->data->CheckCollision(App->input->GetMousePosition()))
		{
			return item->data;
		}
	}
	return NULL;
}
// class Gui ---------------------------------------------------*/

