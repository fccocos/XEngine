#pragma once
#include "log.h"
#include "Application.h"

#ifdef XE_PLATFORM_WINDOWS

extern  XEngine::Application* XEngine::CreateApplication();

int main(int argc, char** argv)
{
	XEngine::Log::Init();
	auto app = XEngine::CreateApplication();
	app->run();
	delete app;
}

#endif
