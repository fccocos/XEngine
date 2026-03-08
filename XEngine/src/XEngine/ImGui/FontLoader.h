#pragma once
#include <imgui.h>
#include <string>
#include <array>
#include <unordered_map>
namespace XEngine {
	class FontLoader
	{
	public:
		static const unsigned int MaxSize = 64;
		static const unsigned int MaxFontTypeSize = 48;
		struct FontSet {
			std::array<ImFont*, MaxSize> FontHandles;
		};
		FontLoader(const std::array<std::string, MaxFontTypeSize>& pathArray);
		FontLoader(const std::string& path);

		void SetFontConfig(const ImFontConfig& config) { m_FontConfig = config; }
		const ImFontConfig& GetFontConfig() const { return m_FontConfig; }

		ImFont* GetFont(std::string fontName, unsigned int fontSize);
		void AddFont(std::string fontName, ImFont* fontPtr);
		void RemoveFont(std::string fontName);

		void PushFont(std::string fontName, unsigned int fontSize);
		void PopFont();
		const std::unordered_map<std::string, Ref<FontSet>>& GetFontNameSizeMap() const { return m_FontNameSizeMap; }
		
	private:
		void InitConfig();
		ImFont* setFont(std::string fontPath, unsigned int fontSize);
	private:
		ImFontConfig m_FontConfig;
		ImGuiIO& m_IO = ImGui::GetIO();
		std::unordered_map<std::string, Ref<FontSet>> m_FontNameSizeMap;
		//ImFont* m_currrentGlobalFont;
	};
}


