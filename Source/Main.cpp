#include <stdlib.h>
#include "Application.h"
#include "./Modules/ModuleRender.h"
#include "Globals.h"

#include "./lib/SDL/include/SDL.h"
#pragma comment( lib, "./lib/SDL/lib/x64/SDL2.lib" )
#pragma comment( lib, "./lib/SDL/lib/x64/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,  
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			AVISO("Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			AVISO("Application Init --------------");
			if (App->Init() == false)
			{
				AVISO("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				AVISO("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				AVISO("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			AVISO("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				AVISO("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}


	delete App;
	AVISO("Bye :)\n");
	return main_return;
}
