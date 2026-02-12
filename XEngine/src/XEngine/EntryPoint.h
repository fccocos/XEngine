#pragma once

#ifdef XE_PLATFORM_WINDOWS

extern XEngine::Application* XEngine::CreateApplication();

int main(int argc, char** argv)
{
	XEngine::Log::Init();
	XE_CORE_ERROR("XEngine::OpenGL::GSLS::Compile Fatal");
	XE_CORE_INFO("XEngine::Render::Layer::Compiler::Linking error");
	int a = 4;
	XE_TRAC("Hello, Var a={0}", a);

	auto app = XEngine::CreateApplication();
	app->run();
	delete app;
}

#endif
