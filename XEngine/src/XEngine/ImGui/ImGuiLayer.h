#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "XEngine/Core/Layer.h"

#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/Event/MouseEvent.h"
#include "XEngine/Event/KeyEvent.h"

#include "FontLoader.h"

namespace XEngine {
	class XENGINE_API ImGuiLayer: public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override; // 初始化
		void OnDetach() override; // 接受

		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetDarkTheme();

		void BlocEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
		Scope<FontLoader> m_FontLoader;
	};

	struct FontConfig {
		std::array<std::string, FontLoader::MaxFontTypeSize> pathArray{
			"Assert/Fonts/MonaSansDisplayExpanded-Light.ttf",
			"Assert/Fonts/SourceHanSerifSC-SemiBold.otf",
			"Assert/Fonts/SmileySans-Oblique.ttf"
		};
	};
}


