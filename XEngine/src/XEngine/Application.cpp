#include "xepch.h"

#include "Application.h"

#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/log.h"
#include "spdlog/fmt/ostr.h"

#include <glad/glad.h>

namespace XEngine {

// 事件绑定宏
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1) 

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
	
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e) {
		// 用事件分发器分发事件
		EventDispather dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		XE_CORE_INFO("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled()) {
				break;
			}
		}

	}

	void Application::run() {
		while (m_Running) {
			glClearColor(0.2, 0.3, 0.3, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}