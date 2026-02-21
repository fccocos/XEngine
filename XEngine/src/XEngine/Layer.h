#pragma once
#include "XEngine/core.h"
#include "XEngine/Event/Event.h"
namespace XEngine {
	class XENGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // 初始化
		virtual void OnDetach() {} // 接受
		virtual void OnUpdate() {} // 更新，每针都会发生
		virtual void OnImGuiRender() {} // ImGui渲染
		virtual void Begin() {};
		virtual void End() {};
		virtual void OnEvent(Event& event) {}  // 每一层的事件注册

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}

