#pragma once
#include "core.h"
#include "Event/Event.h"
#include "XEngine/Window.h"
#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/LayerStack.h"

#include "XEngine/ImGui/ImGuiLayer.h"

#include "XEngine/Renderer/Shader.h"

#include "XEngine/Renderer/Buffer.h"

#include "XEngine/Renderer/VertexArray.h"

#include "XEngine/Renderer/OrthographicCamera.h"

#include "XEngine/Core/TimeStep.h"

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

		static inline Application& Get() { return *s_Instance; }

		inline Window& GetWindow() const { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


