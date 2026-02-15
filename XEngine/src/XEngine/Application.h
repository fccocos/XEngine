#pragma once
#include "core.h"
#include "Event/Event.h"
#include "XEngine/Window.h"
#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/LayerStack.h"

namespace  XEngine {
	class XENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void OnEvent(Event& e); // 注册事件函数
		void run();

		void PushLayer(Layer* layer); // 压入层
		void PushOverlay(Layer* layer); // 压入覆盖层，具有优先渲染权
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


