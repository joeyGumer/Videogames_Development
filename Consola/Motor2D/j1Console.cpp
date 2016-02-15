#include "j1Console.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Fonts.h"



j1Console::j1Console() : j1Module()
{

}


j1Console::~j1Console()
{}


bool j1Console::Awake(pugi::xml_node&)
{

	return true;
}

bool j1Console::Start()
{
	//Put it with the width and height of the screen
	//This is an example
	message_img = App->gui->AddGuiImage({ 0, 0 }, { 0, 0, 1024, 200 }, { 100, 100, 100, 200 }, NULL, this);
	message_img->visible = false;

	input = App->gui->AddGuiInputBox("COMANDOS", App->font->default, { 0, 200 }, 1024, { 0, 0, 1024, 20 }, { 50, 50, 50, 200 }, { 0, 0 }, NULL, this);
	input->visible = false;
	input->interactable = false;
	input->focusable = false;
	/*input_img = App->gui->AddGuiImage({ 0, 200 }, { 0, 0, 1024, 20 }, { 50, 50, 50, 200 }, NULL, this);
	input_img->visible = false;*/

	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

bool j1Console::PreUpdate()
{
	return true;
}


bool j1Console::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		message_img->visible = !message_img->visible;
		input->visible = input->interactable = input->focusable = !input->visible;
	}
	return true;
}


