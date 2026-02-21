#include "xepch.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace XEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle) {
		XE_CORE_ASSERT(windowHandle, "WindowHandle is null!");
	}
	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		XE_CORE_ASSERT(status, "Failed to initilize Glad");
	}
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}