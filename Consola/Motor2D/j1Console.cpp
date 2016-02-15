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
	message_img = App->gui->AddGuiImage({ 0, 0 }, { 0, 0, 1024, CONS_HEIGHT }, { 100, 100, 100, 200 }, NULL, this);
	message_img->visible = false;

	input = App->gui->AddGuiInputBox("COMANDOS", App->font->default, { 0, CONS_HEIGHT }, 1024, { 0, 0, 1024, INTERLINE }, { 50, 50, 50, 200 }, { 0, 0 }, NULL, this);
	input->visible = false;

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

bool j1Console::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		message_img->visible = !message_img->visible;
		input->visible = input->interactable = input->focusable = !input->visible;
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		//App->gui->AddGuiLabel
		messages.add(App->gui->AddGuiLabel(input->sent_text, App->font->default, { 0, CONS_HEIGHT}, message_img, this));
		

		for (p2List_item<GuiLabel*>* item = messages.start; item; item = item->next)
		{
			iPoint pos = item->data->GetLocalPosition();
			item->data->SetLocalPosition({pos.x, pos.y - INTERLINE});
		}
	}
	return true;
}

bool j1Console::PostUpdate()
{

	return true;
}

void j1Console::OnEvent(GuiElement* element, GUI_Event even)
{
	/*if (element->type == GUI_INPUTBOX)
	{
		switch (even)
		{
		case EVENT_INPUT_CHANGE:
			last_msg = input->text.text;
			last_msg.GetCapacity();
		break;

		}
	}*/
}


