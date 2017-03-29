#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"

//
//	ENGINE - This is where the engine starts.
//
void main ()
{
	//
	//	We create a new application .
	//
	Application* theApp = new Application();

	//
	//	Game loop - This keeps us in the game, so that we don't just leave straight away but wait for some form of user input to leave the game/application.
	//
	//	This is also the startup where everything necessary that needs to be initialised.
	//
	if (theApp->Startup())
	{
		//
		//	This is the update function to call everything else's update.
		//
		while (theApp->Update())
		{
			//
			//	After everything has been updated we call the draw function on everything.
			//
			theApp->Draw();
		}

		//
		//	Once the game loop has been closed we call the destructor on everything.
		//
		theApp->Shutdown();
	}
}