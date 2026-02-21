#pragma once

#include "xepch.h"

#include "XEngine/core.h"
#include "XEngine/Event/Event.h"

namespace XEngine {
	// 窗口属性
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& t = "XEngine Engine",
			unsigned int w = 1280,
			unsigned int h = 720) :
			Title(t), Width(w), Height(h) {
		}

	};

	class XENGINE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// attributes
		virtual void SetEventCallback(const EventCallbackFn& callabck) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
