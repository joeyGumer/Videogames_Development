#ifndef __j1CONSOLE__
#define __j1CONSOLE__

#include "j1Module.h"
#include "j1Gui.h"
#include "GuiElements.h"

#include "p2DynArray.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1App.h"

#define LINE_SPACING 16

enum cVarTypes
{
	e_float,
	e_int,
	e_string,
	e_bool
};

// Command----------------------

class Command
{
public:
	Command(const char* name, uint numberOfArguments)
	{
		commandName = name;
		nArgs = numberOfArguments;
	}

private:
	j1Module* listener = NULL;
	uint nArgs;
	p2SString commandName;

public:
	p2SString GetName() 
	{ 
		return commandName;
	}
	uint GetNArgs()
	{ 
		return nArgs;
	}
	j1Module* GetListener() 
	{
		return listener; 
	}
	void SetListener(j1Module* _listener)
	{
		listener = _listener;
	}
	virtual void Function(p2DynArray<p2SString>* args) {}
};

//------------------------------

// CVar-------------------------

class cVar
{
public:
	cVar(const char* _name, float* _reference, bool _serialize = false);
	cVar(const char* _name, int* _reference, bool _serialize = false);
	cVar(const char* _name, char* _reference, bool _serialize = false);
	cVar(const char* _name, bool* _reference, bool _serialize = false);

	bool serialize;

private:
	p2SString name;
	cVarTypes type;
	Command* linkedFunction = NULL;

	union 
	{
		float* floatRef;
		int* intRef;
		char* stringRef;
		bool* boolRef;
	} reference;

	union 
	{
		float floatVar;
		int intVar;
		char* stringVar;
		bool boolVar;
	} value;

public:
	void LinkCommand(Command* toLink);

	void Set(float _value);
	void Set(int _value);
	void Set(char* _value);
	void Set(bool _value);
	void Set(p2SString data);

	cVarTypes GetType();
	p2SString GetName();

	void Read(void* ret, cVarTypes expectedData);
	bool Read(float* output);
	bool Read(int* output);
	bool Read(char* output);
	bool Read(bool* output);
	void* ForceRead();

	void Display();
};

//------------------------------

// Console----------------------

class j1Console : public j1Module
{
private:
	//Commands -----------------
	p2List<Command*> commandsList;

	//Cvars -----------------------
	p2List<cVar*> cVars;

	//Visual
	GuiInputBox* inputBox;

	p2DynArray<GuiLabel*> output;
	int outputOffset = 0;

	GuiImage* consoleRect;
	GuiImage* inputRect;

	//Other
	bool closeGame = false;

public:
	//Methods
	uint AddCommand(Command* commandToAdd, j1Module* listener);
	uint AddCVar(const char* _name, float* reference, bool serialize = false);
	uint AddCVar(const char* _name, int* reference, bool serialize = false);
	uint AddCVar(const char* _name, char* reference, bool serialize = false);
	uint AddCVar(const char* _name, bool* reference, bool serialize = false);

	void Execute(p2SString input);
	void ConsoleOutput(p2SString toAdd);

	void DisplayCommands();

private:
	void Cut(p2SString input, p2DynArray<p2SString>* output);
	Command* FindCommand(p2SString name, uint nArgs);
	cVar* FindCVar(p2SString name);
	void SetCVar(p2SString value);



public:
	//Base methods
	j1Console();

	// Destructor
	virtual ~j1Console();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	//Update
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool SaveCVars(pugi::xml_node& data) const;
	bool LoadCVars(pugi::xml_node&);

	void ReceiveEvent(GuiElement* el, GUI_Event ev);


private:
#pragma region Commands
	struct Command_List : public Command
	{
		Command_List() : Command("list", 0){}
		void Function(p2DynArray<p2SString>* args)
		{
			App->console->DisplayCommands();
		}
	};
	Command_List c_List;

	
	struct Command_Update_Times : public Command
	{
		Command_Update_Times() : Command("update_times", 0){}
		void Function(p2DynArray<p2SString>* args)
		{
			App->showTimes = true;
		}
	};
	Command_Update_Times c_Update_Times;
	

	struct Command_Quit : public Command
	{
		Command_Quit() : Command("quit", 0){}
		void Function(p2DynArray<p2SString>* args)
		{
			App->console->closeGame = true;
		}
	};
	Command_Quit c_Quit;

	struct Command_Save : public Command
	{
		Command_Save() : Command("save", 0){}
		void Function(p2DynArray<p2SString>* args)
		{
			App->SaveGame("save_game.xml");
		}
	};
	Command_Save c_Save;

	struct Command_Load : public Command
	{
		Command_Load() : Command("load", 0){}
		void Function(p2DynArray<p2SString>* args)
		{
			App->LoadGame("save_game.xml");
		}
	};
	Command_Load c_Load;
#pragma endregion

};

#endif