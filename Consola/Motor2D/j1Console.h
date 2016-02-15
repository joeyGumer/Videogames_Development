#ifndef __J1CONSOLE_H__
#define __J1CONSOLE_H__

#include "j1Module.h"

class GuiImage;
class GuiInputBox;

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

	//Called after all updates
	bool PostUpdate();

	//Called before quitting
	bool CleanUp();

private:
	GuiImage* message_img;
	GuiInputBox* input;
};












#endif __J1CONSOLE_H__