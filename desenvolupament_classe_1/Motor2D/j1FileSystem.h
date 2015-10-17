#ifndef _J1FILESYSTEM_H_
#define _J1FILESYSTEM_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

#pragma comment ( lib, "PhysFS/libx86/physfs.lib" )


class j1FileSystem : public j1Module
{
	public:
		//constructor
		j1FileSystem(const char* game_path);

		//Destructor
		~j1FileSystem();

		//Called before the first frame
		bool Start();

		//Called each cycle
		bool Update();

		//Called before quitting
		bool CleanUp();
		
		int LoadFile(const char* file, char** buffer);
		SDL_RWops* LoadFile(const char* file);

		bool AddPath(const char* game_path);
};

#endif _J1FILESYSTEM_H_
