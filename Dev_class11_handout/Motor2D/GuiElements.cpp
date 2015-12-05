#include "GuiElements.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Input.h"


#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )



//Constructors 
//May i change this in the future for the rect
GuiElement::GuiElement(iPoint p, GUI_Type t, GuiElement* par = NULL, j1Module* list = NULL) : type(t), parent(par), listener(list), mouseIn(false)
{
	local_rect = { p.x, p.y, tex_rect.w, tex_rect.h };
	interactable = false;
	focusable = false;
	draggable = false;
}

GuiElement::GuiElement(iPoint p, SDL_Rect r, GUI_Type t, GuiElement* par, j1Module* list) : parent(par), tex_rect(r), type(t), listener(list), mouseIn(false)
{
	local_rect = { p.x, p.y, tex_rect.w, tex_rect.h };
	interactable = false;
	focusable = false;
	draggable = false;
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
void GuiImage::Update()
{
	//put the draw functions here
	Draw();
}

void GuiLabel::Update()
{
	//put the draw functions here
	
	Draw();
}

void GuiInputBox::Update()
{
	Draw();
}
//GuiElement Functions

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

void GuiElement::SetLocalPosition(iPoint p)
{
	local_rect.x = p.x;
	local_rect.y = p.y;
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

bool GuiElement::CheckEvent(GuiElement* hover, GuiElement* focus)
{
	bool inside = (hover == this);
	bool focused = (focus == this);
	
	//May this go outside of the function?
		if (inside != mouseIn)
		{
			if (listener)
			{
				if (inside)
					listener->OnEvent(this, EVENT_MOUSE_ENTER);
				else
					listener->OnEvent(this, EVENT_MOUSE_EXIT);
			}
			mouseIn = inside;
		}
		
		if (inside)
		{
			if (listener)
			{
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_DOWN);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_UP);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
					listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_DOWN);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
					listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_UP);
			}

			if (draggable && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				iPoint p = GetLocalPosition();
				iPoint m = App->input->GetMouseMotion();
				SetLocalPosition(p + m);
			}
		}

		if (focusIn != focused)
		{
			if (listener)
			{
				if (focused)
					listener->OnEvent(this, EVENT_FOCUS_DOWN);
				else
					listener->OnEvent(this, EVENT_FOCUS_UP);
			}
			focusIn = focused;
		}

		if (focused)
		{
			if (listener)
			{
				if(App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_DOWN);
				if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_UP);
			}
		}
	
	return true;
}

void GuiElement::DrawDebug()
{
	SDL_Rect rect = GetScreenRect();
	rect.x -= App->render->camera.x;
	rect.y -= App->render->camera.y;

	App->render->DrawLine(rect.x, rect.y, rect.x + rect.w, rect.y, 255, 0, 0);
	App->render->DrawLine(rect.x, rect.y, rect.x, rect.y + rect.h, 255, 0, 0);
	App->render->DrawLine(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, 255, 0, 0);
	App->render->DrawLine(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, 255, 0, 0);

}
