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
		GuiElement(iPoint p, GUI_Type t, j1Module* list);
		GuiElement(iPoint p, SDL_Rect r, GUI_Type t, j1Module* list);
		~GuiElement(){}

		virtual void Draw(){}
		virtual bool Update(){ return true; }

		bool CheckCollision(iPoint p);
		bool CheckEvent();
		

		//Utils
		void SetRect(SDL_Rect r){ rect = r; }


	public :
		//Maybe putting a name to identify each element
		GUI_Type	 type;
		//int		 id;
		iPoint		 pos;
		SDL_Rect     rect;
		bool         visible;

		bool		mouseIn;

		j1Module*    listener;
		//For now we'll just use one listener
		//p2List<j1Module*> listeners;
};

class GuiLabel : public GuiElement
{
	public : 
		//put constructors at the cpp
		GuiLabel(p2SString t, _TTF_Font* f, iPoint p, j1Module* list);
		~GuiLabel();

		void Draw();
		//this is provisional
		bool Update();

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
		GuiImage(iPoint p, SDL_Rect r, j1Module* list);
		~GuiImage();

		void Draw();
		bool Update();
};

class GuiButton : public GuiElement
{
	public:
		/*
		GuiButton();
		~GuiButton();
		*/
		void Draw();
		bool Update();
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
	bool Update();
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
	EVENT_MOUSE_LEFTCLICK_DOWN,
	EVENT_MOUSE_LEFTCLICK_UP,
	EVENT_MOUSE_RIGHTCLICK_DOWN,
	EVENT_MOUSE_RIGHTCLICK_UP,
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
	GuiElement* AddGuiImage(iPoint p, SDL_Rect r, j1Module* list);
	GuiElement* AddGuiLabel(p2SString t, _TTF_Font* f, iPoint p, j1Module* list);

	SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__