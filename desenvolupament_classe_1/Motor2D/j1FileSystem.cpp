#include "j1FileSystem.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"

#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

#pragma comment ( lib, "PhysFS/libx86/physfs.lib" )

//constructor
j1FileSystem::j1FileSystem(const char* game_path) : j1Module()
{
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);
	
	//really important for this, don't forget
	AddPath(".");
	AddPath(game_path);
}

//Destructor
j1FileSystem::~j1FileSystem()
{
	PHYSFS_deinit();
}

//Called before the first frame
bool j1FileSystem::Start()
{
	LOG("Loading File System");
	return true;
}

bool j1FileSystem::AddPath(const char* game_path)
{
	bool ret = false;
	//this function creates the files tree!!!!
	if (PHYSFS_mount(game_path, NULL, 1) == 0)
		LOG("File System error while adding a path or zip: %s %s\n", PHYSFS_getLastError(), game_path);
	else
		LOG("File System mounted succsesfully!\n");
		ret = true;

	return ret;
}
//Called each cycle
bool j1FileSystem::Update()
{
	return true;
}

//Called before quitting
bool j1FileSystem::CleanUp()
{
	return true;
}
//preguntar al ricard porque hace doble puntero con el buffer, para evitar el error de que se vuelva usar en la otra funcionç?
int j1FileSystem::LoadFile(const char* file, char** buffer)
{
	int ret = 0;
	
	PHYSFS_file* data = PHYSFS_openRead(file);

	if (data != NULL)
	{
		PHYSFS_sint32 size = PHYSFS_fileLength(data);
		
		if (size > 0)
		{
			*buffer = new char[size];
			int objects = PHYSFS_read(data, *buffer, 1, size);
			
			if (objects != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}

			else
			{
				ret = objects;
			}

			PHYSFS_close(data);
		}
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;

}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

SDL_RWops* j1FileSystem::LoadFile(const char* file)
{
	char* buffer;
	int size = LoadFile(file, &buffer);
	
	if (size>0)
	{ 
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != NULL)
			r->close = close_sdl_rwops;
			
		return r;
		
	}
	else
		return NULL;
}


