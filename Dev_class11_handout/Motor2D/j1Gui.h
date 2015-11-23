#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2
struct SDL_Texture;
// TODO 1: Create your structure of classes
enum UI_Type
{
	UI_LABEL,
	UI_IMAGE,
	UI_BUTTON,
	UI_INPUTBOX
	//UI_CURSOR
};

class UI_Element
{
	public:
		UI_Element();
		~UI_Element();

		virtual bool Draw(){ return true; }
		virtual bool Update(){ return true; }

	public :
		UI_Type      type;
		int			 id;
		iPoint		 pos;
		SDL_Texture* tex;
		bool         visible;
};

class Label : public UI_Element
{
	public : 
		Label();
		~Label();

		bool Draw();
		bool Update();

	public:
		p2SString text;
};

class Image : public UI_Element
{
	public:
		Image();
		~Image();

		bool Draw();
		bool Update();
};

class Button : public UI_Element
{
	public:
		Button();
		~Button();

		bool Draw();
		bool Update();
		//Utils
		bool Execute();
		bool CheckCollision();
	
	public:
		bool isColliding;
		bool clicked;
};

class InputBox : public UI_Element
{
public:
	InputBox();
	~InputBox();

	bool Draw();
	bool Update();
	//Utils
	bool Input();

public:
	//should this be a Label?
	Label text;
	Button button;
	//and a button?
};

//class Cursor : public 


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

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__