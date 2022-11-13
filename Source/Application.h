#ifndef __Application_H__
#define __Application_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleImgui;
class ModuleProgram;
class ModulRenderExercise;
class ModuleDebugDraw;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleImgui* ImGui = nullptr;
	ModuleProgram* program = nullptr;
	ModulRenderExercise* exercise = nullptr;
	ModuleDebugDraw* draw = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;

#endif