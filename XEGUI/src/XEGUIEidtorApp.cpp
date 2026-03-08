#include <xepch.h>
#include <XEngine.h>
#include <XEngine/Core/EntryPoint.h>

#include <glm/gtc/matrix_transform.hpp>

#include "EditorLayer.h"

#include "imgui.h"
namespace XEngine {
	
	class XEGUIEidtor :public Application {
	public:
		XEGUIEidtor():Application("XEGUIEditor") {
			PushLayer(new EditorLayer());
		}
		~XEGUIEidtor() {}
	};



	Application* CreateApplication() {
		return new XEGUIEidtor();
	}
}
