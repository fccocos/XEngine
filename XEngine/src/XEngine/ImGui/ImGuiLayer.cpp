#include "xepch.h"
#include "ImGuiLayer.h"
#include "XEngine/Core/Application.h"

#include "XEngine/Core/core.h"

#include "XEngine/Core/KeyCodes.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>



namespace XEngine {

    static unsigned int g_currentGlabalFontSize = 16;
    static unsigned int g_currentGlabalFontType = 0;
    static std::string  g_currentGlabalFontTypeStr = "SmileySans-Oblique";
	ImGuiLayer::ImGuiLayer():Layer("ImGuiLayer") {

	}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach(){
        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setting font
        io.Fonts->Clear(); // 清除默认字体
        std::array<std::string, FontLoader::MaxFontTypeSize> pathArray{
            "Assert/Fonts/SmileySans-Oblique.ttf",
            "Assert/Fonts/MonaSansDisplayExpanded-Light.ttf",
			"Assert/Fonts/SourceHanSerifSC-SemiBold.otf",
        };
        m_FontLoader = CreateScope<FontLoader>(pathArray);
        // Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();
        // Setup scaling
        SetDarkTheme();
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
        
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
		
	}
    void ImGuiLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        m_FontLoader->PushFont(g_currentGlabalFontTypeStr, g_currentGlabalFontSize);
    }
    void ImGuiLayer::OnImGuiRender() {

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Font Settings")) {
				// font type list
				std::array<std::string, FontLoader::MaxFontTypeSize> fontTypes;
				unsigned int type_iter = 0;
				for (auto& iter : m_FontLoader->GetFontNameSizeMap()) {
					fontTypes[type_iter] = iter.first;
                    type_iter++;

				}
                g_currentGlabalFontTypeStr = fontTypes[g_currentGlabalFontType];

                ImGui::PushID("##type");
				if (ImGui::BeginCombo("Type", g_currentGlabalFontTypeStr.c_str()))
				{
					for (int n = 0; n < type_iter; n++)
					{
						const bool is_selected = (g_currentGlabalFontType == n);
                        if (ImGui::Selectable(fontTypes[n].c_str(), is_selected)) {
                            g_currentGlabalFontType = n;
                            g_currentGlabalFontTypeStr = fontTypes[g_currentGlabalFontType];
                        }
							

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
                ImGui::PopID();

                // font size list
                std::array<std::string, FontLoader::MaxSize> items;
                for (unsigned int i = 0; i < FontLoader::MaxSize; i++) {
                    items[i] = std::to_string(i);
                }
                // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
                const char* combo_preview_value = items[g_currentGlabalFontSize].c_str();
                
                ImGui::PushID("##size");
                if (ImGui::BeginCombo("Size", combo_preview_value))
                {
                    for (int n = 0; n < items.size(); n++)
                    {
                        const bool is_selected = (g_currentGlabalFontSize == n);
                        if (ImGui::Selectable(items[n].c_str(), is_selected))
                            g_currentGlabalFontSize = n;

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopID();
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


    }

    void ImGuiLayer::End() {
        m_FontLoader->PopFont();
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));
        ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_None;
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Update and Render additional Platform Windows
       // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
       //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        

    }


	void ImGuiLayer::SetDarkTheme()
	{
        auto& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        style.Colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        style.Colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Buttons
		style.Colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		style.Colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
		style.Colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
		style.Colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		style.Colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		style.Colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		style.Colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f, 0.9505f, 0.951f, 1.0f };
	
	}

	void ImGuiLayer::OnEvent(Event& e) {
        if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			auto handled = e.Handled() | (e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse);
			e.setHandled(handled);
            
			handled = e.Handled() | (e.IsInCategory(EventCategoryMouseButton) & io.WantCaptureMouse);
			e.setHandled(handled);

			handled = e.Handled() | (e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard);
			e.setHandled(handled);
        }
      
    }
	
	void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}



   
}