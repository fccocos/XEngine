#include "xepch.h"
#include "WindowsWindow.h"

#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/Event/KeyEvent.h"
#include "XEngine/Event/MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace XEngine {
	static bool s_GLFWInitialized = false;

	// glfw错误代码回到函数
	static void GLFWErrorCallback(int error, const char* description) {
		XE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		XE_CORE_INFO("Create window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			// TODO: glfwTernimate on system shutdown
			int success = glfwInit();
			XE_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		XE_CORE_ASSERT(status, "Failed to initilize Glad");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// 设置窗口回调函数
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) {
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = w;
			data.Height = h;
			WindowResizeEvent event(w, h);
			data.EventCallback(event);
		});
		// 设置窗口关闭回调
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		// 设置键盘按键回调
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressEvent ev(key, 0);
					data.EventCallback(ev);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressEvent ev(key, 1);
					data.EventCallback(ev);
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent ev(key);
					data.EventCallback(ev);
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action) {
				case GLFW_PRESS:{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
					break;
				}
				
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(e);
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovementEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(e);
		});
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) glfwSwapInterval(1); else glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

	

}