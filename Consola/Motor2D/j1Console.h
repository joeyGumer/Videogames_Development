#ifndef __J1CONSOLE_H__
#define __J1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"

#define INTERLINE 20
#define CONS_HEIGHT 200

enum GUI_EVENT;

class GuiElement;
class GuiImage;
class GuiInputBox;
class GuiLabel;

class j1Console : public j1Module
{
public:

	//Constructor
	j1Console();

	//Destructor
	~j1Console();

	//Called before render is avaliable
	bool Awake(pugi::xml_node&);

	//Called before the first frame
	bool Start();

	//Called before all updates
	bool PreUpdate();

	//Called each loop iteration
	bool Update(float dt);

	//Called after all updates
	bool PostUpdate();

	//Called before quitting
	bool CleanUp();

	//Check GUI Events
	void OnEvent(GuiElement* element, GUI_Event even);


public:

	p2List<GuiLabel*>	messages;
	GuiImage*			message_img;
	GuiInputBox*		input;
	bool				active;
	
	
};












#endif __J1CONSOLE_H__