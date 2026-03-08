#include "xepch.h"
#include "FontLoader.h"

namespace XEngine {


	static std::string get_filename_no_suffix(const std::string& full_path) {
		std::filesystem::path fs_path(full_path);
		return fs_path.stem().string();
	}

	FontLoader::FontLoader(const std::array<std::string, MaxFontTypeSize>& pathArray) {
		InitConfig();
		for (unsigned int type_iter = 0; type_iter < MaxFontTypeSize; type_iter++) {
			std::string fontType=get_filename_no_suffix(pathArray[type_iter]);
			std::string fontPath = pathArray[type_iter];
			if (fontPath.empty()) {
				break;
			}
			FontSet fonts{};
			for (unsigned int i = 0; i < MaxSize; i++) {
				ImFont* font = setFont(fontPath, i);
				fonts.FontHandles[i] = font;
			}
			m_FontNameSizeMap[fontType] = CreateRef<FontSet>(fonts);
		}
	}

	FontLoader::FontLoader(const std::string& path) {
		InitConfig();
		std::string fontType = get_filename_no_suffix(path);
		FontSet fonts{};
		for (unsigned int i = 0; i < MaxSize; i++) {
			ImFont* font = setFont(path, i);
			fonts.FontHandles[i] = font;
		}
		m_FontNameSizeMap[fontType] = CreateRef<FontSet>(fonts);

	}

	ImFont* FontLoader::GetFont(std::string fontName, unsigned int fontSize) {
		return m_FontNameSizeMap[fontName]->FontHandles[fontSize];
	}

	void FontLoader::AddFont(std::string fontName, ImFont* fontPtr) {
		for (unsigned int i = 0; i < MaxSize; i++)
			m_FontNameSizeMap[fontName]->FontHandles[i] = fontPtr;

	}

	void FontLoader::RemoveFont(std::string fontName) {
		for (auto* font : m_FontNameSizeMap[fontName]->FontHandles) {
			font->~ImFont();
			font = nullptr;
		}
		m_FontNameSizeMap.erase(fontName);

	}

	void FontLoader::PushFont(std::string fontName, unsigned int fontSize) {

		Ref<FontSet> oneTypeFont = m_FontNameSizeMap[fontName];
		ImFont* curFont = oneTypeFont->FontHandles[fontSize];
		ImGui::PushFont(curFont);
	}

	void FontLoader::PopFont() {
		ImGui::PopFont();
	}
	void FontLoader::InitConfig() {
		m_IO.Fonts->Clear();
		m_FontConfig.PixelSnapH = true;
	}

	ImFont* FontLoader::setFont(std::string fontPath, unsigned int fontSize) {
		return m_IO.Fonts->AddFontFromFileTTF(
			fontPath.c_str(),
			static_cast<float>(fontSize),
			&m_FontConfig,
			m_IO.Fonts->GetGlyphRangesChineseFull());
	}
}


