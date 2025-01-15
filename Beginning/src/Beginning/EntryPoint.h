#pragma once

#undef main

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//CLASS DEFINITION: Initializes the application and starts the game loop.

extern Beginning::Application* Beginning::CreateApplication();

int main(int argc, char** argv)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(594);	617
	//_CrtSetBreakAlloc(617);	

		//std::unique_ptr<Beginning::Application> app(Beginning::CreateApplication());
	{
		Beginning::Application* app = Beginning::CreateApplication();
		app->Run();
		delete app;
		app = nullptr;
	}
	_CrtDumpMemoryLeaks();
	return 0;
}

