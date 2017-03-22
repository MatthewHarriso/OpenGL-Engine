#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"

void main ()
{
	Application* theApp = new Application();

	if (theApp->Startup())
	{
		while (theApp->Update())
		{
			theApp->Draw();
		}

		theApp->Shutdown();
	}
}