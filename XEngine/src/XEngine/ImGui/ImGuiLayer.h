#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "XEngine/Core/Layer.h"

#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/Event/MouseEvent.h"
#include "XEngine/Event/KeyEvent.h"


namespace XEngine {
	class XENGINE_API ImGuiLayer: public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override; // 初始化
		void OnDetach() override; // 接受

		void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}


