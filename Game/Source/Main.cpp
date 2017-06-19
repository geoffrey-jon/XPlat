#include "XPlat.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,	PSTR cmdLine, int showCmd)
{
	XPlat MainApplication(hInstance);

	if (!MainApplication.Init())
		return 0;

	return MainApplication.Run();
}