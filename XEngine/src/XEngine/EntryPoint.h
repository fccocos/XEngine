#pragma once
#include <stdio.h>

#ifdef XE_PLATFORM_WINDOWS

extern XEngine::Application* XEngine::CreateApplication();

int main(int argc, char** argv)
{
	printf("XEngine engine\n");
	auto app = XEngine::CreateApplication();
	app->run();
	delete app;
}

#endif
