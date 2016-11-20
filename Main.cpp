#include "DCBase.h"
#include "DCLogger.h"
#include "DCPlatform.h"
#include <Windows.h>
#include <crtdbg.h>

using namespace DevilsCanvas;

extern "C" int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	LOG_TO_FILE(Application Started);
	DCPlatform* platform = NULL;
	ReturnStatus rs = DCPlatform::CreatePlatform(L"First Test Window",".\\Resources\\Configuration.config", &platform);
	if (ReturnStatus::eError == rs || NULL == platform)
	{
		LOG_TO_FILE(Error occurred in platform creation);

		if (platform)
			delete platform;
		platform = NULL;

		return rs;
	}

	platform->EnterEventLoop();

	if (platform)
		delete platform;
	platform = NULL;

	LOG_TO_FILE(Application Ended);

	_CrtDumpMemoryLeaks();

	return 0;
}
