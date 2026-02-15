#pragma once

#include "Event.h"


namespace XEngine {
	class XENGINE_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int w, unsigned int h) :m_Width(w), m_Height(h) {}
		[[nodiscard]] inline unsigned int GetWidth() const { return m_Width; }
		[[nodiscard]] inline unsigned int GetHeigth() const { return m_Height; }

		[[nodiscard]] std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << GetWidth() << " ," << GetHeigth();
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};
	
	class XENGINE_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class XENGINE_API AppTickEvent :public Event {
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/*class XENGINE_API AppUpdateEvent :public Event {
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};*/

	class XENGINE_API AppRenderEvent :public Event {
	public:
		AppRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}