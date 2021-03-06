#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1FileSystem.h"
#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	fs = new j1FileSystem();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();

	config_file.reset();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	bool ret = LoadConfig();
	
	// self-config
	title.create(app_config.child("title").child_value());
	organization.create(app_config.child("organization").child_value());

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
bool j1App::LoadConfig()
{
	bool ret = true;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = config_file.child("config");
		app_config = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
}

// ---------------------------------------------
void j1App::FinishUpdate()
{

	// TODO 1: This is a good place to call load / Save functio

	if (want_to_save)
	{
		SavegameNow();
		want_to_save = false;
	}

	if (want_to_load)
	{
		LoadGameNow();
		want_to_load = false;
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

//Load/Save functions
//These two are called when you say you want to load/save
bool j1App::Load(const char* file)
{
	want_to_load = true;
	load_game.create("save/%s",file);
	
	return true;
}

bool j1App::Save(const char* file) const
{
	LOG("Comfirming to Save the Game state");
	want_to_save = true;
	save_game.create(file);

	
	
	return true;
}

//These two actually Load/Save
bool j1App::LoadGameNow()
{
	LOG("Loading %s", load_game.GetString());
	
	pugi::xml_document	state_file;
	pugi::xml_node		state;
	bool				ret = false;
	char*				buf;

	uint size = App->fs->Load(load_game.GetString(), &buf);

	if (size > 0)
	{
		pugi::xml_parse_result result = state_file.load_buffer(buf, size);
		RELEASE(buf);

		if (result != NULL)
		{
			state = state_file.child("state");

			p2List_item<j1Module*>* item = modules.start;
			ret = true;

			while (item != NULL && ret == true)
			{
				item->data->Load(state.child(item->data->name.GetString()));
				item = item->next;
			}

			state_file.reset();
			if (ret == false)
			{
				LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
			}
		}
		else
		{
			LOG("Could not load map xml file state.xml. pugi error: %s", result.description());
		}
	}
	else
	{
		LOG("Could not load game state xml file %s", load_game.GetString());
	}

	return ret;
	
}

bool j1App::SavegameNow() const
{
	LOG("Saving Game State to %s...", save_game.GetString());

	bool ret = true;

	pugi::xml_document data;
	pugi::xml_node data_root = data.append_child("state");
	pugi::xml_node node;


	p2List_item<j1Module*>* item = modules.start;

	//this cycle, creates all the nodes childs of data_root, that are the module, and at the same time, saves it's state, configuring the xml
	while (item != NULL && ret == true)
	{
		node = data_root.append_child(item->data->name.GetString());
		ret = item->data->Save(node);
		item = item->next;
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);
		LOG("%s", stream.str().c_str());
		//Don't know how to do this lul
		
		App->fs->Save(save_game.GetString(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving %s", save_game.GetString());
	}
	else
	{
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}

	data.reset();

	return ret;
}

// TODO 3: Create a simulation of the xml file to read 



// TODO 4: Create a method to actually load an xml file
// then call all the modules to load themselves

// TODO 7: Create a method to save the current state
// First fill a pugui::xml_document
// Then put it all in memory with
// std::stringstream stream;
// my_xml_document.save(stream);
// then access it via stream.str().c_str()
