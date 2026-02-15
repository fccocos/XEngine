#pragma once

#include "XEngine/core.h"


namespace XEngine {
	// Events in XEngine are currently blocking, meaning when an event
	// occurs it immediately gets dispatched and must be dealt with rigth
	// then an there.
	// For the future, a better strategy might be to buffer events in an
	// event bus and process them during the “event" part of the udpate stage

	enum class EventType {
		None = 0,                                                             // 没有事件发生
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, // 窗口事件
		AppTick, AppUpdate, AppRender,                                        // App事件
		KeyPressed, KeyReleased,                                              // 按键事件
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,   // 鼠标事件
	};

	// 事件分类
	enum EventCategory {
		None = 0,
		EventCategoryApplication   = BIT(0),
		EventCategoryInput         = BIT(1),
		EventCategoryKeyboard      = BIT(2),
		EventCategoryMouse		   = BIT(3),
		EventCategoryMouseButton   = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	
	/// <summary>
	/// Event是一个抽象类，是其他事件类的接口
	/// </summary>
	class XENGINE_API Event { 
		friend class EventDispather;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0; // 只用调试
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool Handled() const { return m_Handled; }
		inline bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; } // 用于判别事件所属分类
	protected:
		bool m_Handled = false; // 用于标志事件是否被处理了
	};
	class EventDispather {
		template<typename T>
		using EventFn = std::function<bool(T&)>; // 函数对象，又叫仿函式
	public:
		EventDispather(Event& event) :m_Event(event) {}
		
		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	
	private:
		Event& m_Event;

	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
								
}
