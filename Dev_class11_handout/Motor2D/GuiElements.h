#ifndef __GUIELEMENTS_H__
#define __GUIELEMENTS_H__

#include "j1Module.h"
#include "p2Point.h"
//I'm doing trap here...
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct _TTF_Font;

enum GUI_Type
{
	GUI_LABEL,
	GUI_IMAGE,
	GUI_BUTTON,
	GUI_INPUTBOX
};

enum GUI_Event
{
	EVENT_MOUSE_LEFTCLICK_DOWN,
	EVENT_MOUSE_LEFTCLICK_UP,
	EVENT_MOUSE_RIGHTCLICK_DOWN,
	EVENT_MOUSE_RIGHTCLICK_UP,
	EVENT_MOUSE_ENTER,
	EVENT_MOUSE_EXIT,
	EVENT_FOCUS_DOWN,
	EVENT_FOCUS_UP
};

class GuiElement
{
public:
	GuiElement(iPoint p, GUI_Type t, GuiElement* par, j1Module* list);
	GuiElement(iPoint p, SDL_Rect r, GUI_Type t, GuiElement* par, j1Module* list);
	~GuiElement(){}

	virtual void Draw(){}
	virtual void Update(){}

	//Utils
	bool CheckCollision(iPoint p);
	bool CheckEvent(GuiElement* hover, GuiElement* focus);
	void DrawDebug();


	//Utils
	iPoint GetLocalPosition();
	iPoint GetScreenPosition();
	SDL_Rect GetScreenRect();
	SDL_Rect GetLocalRect();
	void SetLocalPosition(iPoint p);

	void SetTextureRect(SDL_Rect r){ tex_rect = r; }
	void SetLocalRect(SDL_Rect r){ local_rect = r; }


public:
	//Maybe putting a name to identify each element
	GUI_Type	 type;
	//int		 id;
	GuiElement*  parent;
	//not defined
	bool         visible;
	bool		 mouseIn;
	bool		 interactable;
	bool		 draggable;
	bool		 focusable;
	bool		 focusIn;
	//it can be mouseIn but for sure, i put a different variable, "selected"
	j1Module*    listener;

	//passar a private
	SDL_Rect     tex_rect;

private:
	SDL_Rect     local_rect;
	//For now we'll just use one listener
	//p2List<j1Module*> listeners;
};

class GuiLabel : public GuiElement
{
public:
	//TODO: Manage the font size
	GuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, j1Module* list);
	~GuiLabel(){}

	void Draw();
	//this is provisional
	void Update();

	//Utils
	void SetText(p2SString t){ text = t; }

public:
	p2SString text;
	//TODO: have to destroy the texture created for the text
	_TTF_Font* font;
	SDL_Texture* tex;
};

class GuiImage : public GuiElement
{
public:
	GuiImage(iPoint p, SDL_Rect r, GuiElement* par, j1Module* list);
	~GuiImage(){};

	void Draw();
	void Update();
};

class GuiInputBox : public GuiElement
{
public:

	GuiInputBox(p2SString t, _TTF_Font* f, iPoint p, SDL_Rect r, GuiElement* par, j1Module* list);
	~GuiInputBox(){};


	void Draw();
	void Update();
	//Utils
	bool Input();

public:
	//should this be a Label?
	GuiLabel text;
	GuiImage image;
	//and a button?
};



#endif _GUIELEMENTS_H_