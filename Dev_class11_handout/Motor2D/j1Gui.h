#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2
struct SDL_Texture;
struct _TTF_Font;

enum GUI_Type
{
	GUI_LABEL,
	GUI_IMAGE,
	GUI_BUTTON,
	GUI_INPUTBOX
	//UI_CURSOR
};

class GuiElement
{
	public:
		GuiElement(iPoint p, GUI_Type t): pos(p), type(t), mouseIn(false){}
		GuiElement(iPoint p, SDL_Rect r, GUI_Type t) : pos(p), rect(r), type(t), mouseIn(false){}
		~GuiElement(){}

		virtual void Draw(){}
		virtual bool Update(iPoint p){ return true; }

		bool CheckCollision(iPoint p);

	public :
		GUI_Type	 type;
		//int		 id;
		iPoint		 pos;
		SDL_Rect     rect;
		bool         visible;

		bool		mouseIn;

		p2List<j1Module*> listeners;
};

class GuiLabel : public GuiElement
{
	public : 
		GuiLabel(p2SString t, _TTF_Font* f, iPoint p) : GuiElement(p, GUI_LABEL), text(t), font(f){}
		~GuiLabel();

		void Draw();
		//this is provisional
		bool Update(iPoint p){ mouseIn = CheckCollision(p); return true; }

	public:
		p2SString text;
		//TODO: have to destroy the texture created for the text
		_TTF_Font* font;
};

class GuiImage : public GuiElement
{
	public:
		GuiImage(iPoint p, SDL_Rect r) :GuiElement(p, r, GUI_IMAGE){}
		~GuiImage();

		void Draw();
		bool Update(iPoint p){ mouseIn = CheckCollision(p); return true; }
};

class GuiButton : public GuiElement
{
	public:
		/*
		GuiButton();
		~GuiButton();
		*/
		void Draw();
		bool Update(iPoint p){ mouseIn = CheckCollision(p); return true; }
		//Utils
		//bool Execute();

	
	public:
		bool isColliding;
		bool clicked;

		GuiImage image;
};

class GuiInputBox : public GuiElement
{
public:
	/*
	GuiInputBox();
	~GuiInputBox();
	*/

	void Draw();
	bool Update(iPoint p){ mouseIn = CheckCollision(p); return true; }
	//Utils
	bool Input();

public:
	//should this be a Label?
	GuiLabel text;
	GuiButton button;
	//and a button?
};

//class Cursor : public 

enum GUI_Event
{
	EVENT_MOUSE_CLICK,
	//don't use EVENT_MOUSE_COLLIDE because it happens on each frame
	EVENT_MOUSE_ENTER,
	EVENT_MOUSE_EXIT,
	//... I'll be expanding this

};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	GuiElement* AddGuiImage(iPoint p, SDL_Rect r);
	GuiElement* AddGuiLabel(p2SString t, _TTF_Font* f, iPoint p);

	SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__