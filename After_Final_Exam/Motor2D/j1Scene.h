#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GuiElement;
class GuiLabel;
class GuiImage;
class GuiInputBox;
class GuiSlider;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Called on certain event
	void OnEvent(GuiElement* element, GUI_Event even);

private:
	SDL_Texture* debug_tex;


	//Scene Elements
	//GuiImage* window;
	/*
	GuiImage* button;
	*/
	GuiLabel* button_title;
	GuiLabel* window_title;
	/*
	GuiInputBox* input;
	GuiInputBox* input2;*/
	//
	//GuiSlider* slider;
	//GuiLabel* value_label;
	//p2List<GuiLabel*> labels;

	pugi::xml_node scene_node;
};

#endif // __j1SCENE_H__