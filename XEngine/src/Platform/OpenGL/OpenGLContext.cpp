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
		LOG_DEBUG_SHORT("\t\tOpenGL Info ");
		LOG_DEBUG_SHORT("\tVendor:\t\t{0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		LOG_DEBUG_SHORT("\tRenderer:\t{0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		LOG_DEBUG_SHORT("\tVersion:\t{0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

#ifdef XE_ENABLE_ASSERTS
		int versionMajor{};
		int versionMinor{};
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		XE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "XEngine requires at least OpenGL Version 4.5");
		
#endif // XE_ENABLE_ASSERTS


		
	}
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}