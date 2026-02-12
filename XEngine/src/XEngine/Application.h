#pragma once
#include "core.h"

namespace  XEngine {
	class XENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


