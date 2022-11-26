#pragma once
#ifndef __MODULETIMER__
#define __MODULETIMER__

#include "Module.h"

class Application;

class ModuleTimer : public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	float getDeltaTime() { return deltaTime; }
	
private:

	float previousTime = 0.0f;
	float deltaTime = 0.0f;

};


#endif
