#pragma once
#include "XEngine/Debug/Instrumentor.h"
#include "log.h"
#include "Application.h"

#include "XEngine/Renderer/ParticleSystem/Random.h"
#include "XEngine/Core/Utils.h"

#ifdef XE_PLATFORM_WINDOWS

extern  XEngine::Application* XEngine::CreateApplication();

int main(int argc, char** argv)
{

	XEngine::Log::Init();

	XE_PROFILE_BEGIN_SESSION("Startup", "XEngineProfile-Stratup.json");
	auto app = XEngine::CreateApplication();
	XE_PROFILE_END_SESSION();

	XE_PROFILE_BEGIN_SESSION("Runtime", "XEngineProfile-Runtime.json");
	app->run();
	XE_PROFILE_END_SESSION();

	XE_PROFILE_BEGIN_SESSION("Shutdown", "XEngineProfile-Shutdown.json");
	delete app;
	XE_PROFILE_END_SESSION();
}

#endif
