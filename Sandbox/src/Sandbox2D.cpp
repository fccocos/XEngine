#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>


Sandbox2D::Sandbox2D():
	Layer("Example"), 
	m_CameraController(3840.0f / 2160.0f, true) {
	
}

void Sandbox2D::OnAttach() {
	m_Texture2D = XEngine::Texture2D::Create("Assert/Textures/container.jpg");
	m_SpriteSheet = XEngine::Texture2D::Create("Assert/Games/Textures/tilemap_packed.png");
	m_TilingRoad = XEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0.0f,1.0f }, { 16.0f,16.0f }, {3.0f, 3.0f});
	
	m_CameraController.GetCamera().SetRotation(90.0f);

	XEngine::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = XEngine::Framebuffer::Create(fbSpec);
	
#if TEST1
	m_Texture2D = XEngine::Texture2D::Create("Assert/Textures/container.jpg");
	m_FireTexture = XEngine::Texture2D::Create("Assert/Textures/rain.png");

	m_RainRules = std::make_shared<XEngine::ParticleRules>(XEngine::ParticlePresets::rain());
	m_FireRules = std::make_shared<XEngine::ParticleRules>(XEngine::ParticlePresets::fire());
	m_PureColorRules = std::make_shared<XEngine::ParticleRules>(XEngine::ParticlePresets::pureColor());

	m_FireRenderer = std::make_unique<XEngine::SpriteParticleRenderer>(m_FireTexture);
	m_RainRenderer = std::make_unique<XEngine::PointParticleRenderer>();
	m_PureColorRenderer = std::make_unique<XEngine::PointParticleRenderer>();


	m_ParcticleManager = std::make_unique<XEngine::ParticleSystemManager>(m_RainRules->m_maxParticles, m_RainRules, std::move(m_RainRenderer));
	m_PureColorParcticleManager = std::make_unique<XEngine::ParticleSystemManager>(m_PureColorRules->m_maxParticles, m_PureColorRules, std::move(m_PureColorRenderer));

	m_ParcticleManager->start();
	m_PureColorParcticleManager->start();

#endif
}

void Sandbox2D::OnDetach() {
#if TEST1
	m_ParcticleManager->stop();
#endif
	
}

void Sandbox2D::OnUpdate(XEngine::TimeStep timeStep) {
	XE_PROFILE_FUNCTION();
	// update
	{
		XE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(timeStep);
	}
	
	XEngine::Renderer2D::ResetStats();
	{
		XE_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->Bind();
		XEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		XEngine::RenderCommand::Clear();
		
	}

	// spirteAlitas
	{
		XE_PROFILE_SCOPE("SpriteAlitas Renderer Draw");
		XEngine::Renderer2D::BeginScense(m_CameraController.GetCamera());

		XEngine::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.8f), glm::vec2(0.1f), m_TilingRoad);

		XEngine::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f), m_SpriteSheet);

		XEngine::Renderer2D::EndScense();

		m_Framebuffer->Unbind();
	}

#if TEST1
	// Particle System
	{
		XE_PROFILE_SCOPE("Particle System  Renderer Draw");

		XEngine::Renderer2D::BeginScense(m_CameraController.GetCamera());
		
		if (m_counter == 1000) {
			m_ParcticleManager->udpateRules(m_FireRules);
			m_ParcticleManager->setRenderer(std::move(m_FireRenderer));
			m_ParcticleManager->burst(100);
		}
		m_counter++;
		m_ParcticleManager->update(timeStep);

		if (XEngine::Input::IsMouseButtonPressed(XE_MOUSE_BUTTON_LEFT)) {
			auto [x, y] = XEngine::Input::GetMousePosition();
			auto width = XEngine::Application::Get().GetWindow().GetWidth();
			auto height = XEngine::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetOrthographicCameraBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();

			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5 - (y / height) * bounds.GetHeight();

			m_PureColorRules->m_emitPosition = {x, y, 0.0f};

			m_PureColorParcticleManager->udpateRules(m_PureColorRules);
			m_PureColorParcticleManager->update(timeStep);
			
		}
		
		XEngine::Renderer2D::EndScense();
	}



	{
		XE_PROFILE_SCOPE("Renderer Draw");
		XEngine::Renderer2D::BeginScense(m_CameraController.GetCamera());

		XEngine::Renderer2D::DrawQuad(glm::vec2(0.5f), glm::vec2(0.5), glm::vec4(0.8, 0.2, 0.3, 1.0f));
		XEngine::Renderer2D::DrawQuad(glm::vec2(-0.5f, 0.5f), glm::vec2(0.5f), glm::vec4(0.3, 0.2, 0.8, 1.0f));
		XEngine::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.8f), glm::vec2(0.8f), glm::vec4(0.3, 0.8, 0.2, 1.0f));
		XEngine::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(0.5f), m_Texture2D);

		XEngine::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.8f), glm::vec2(0.8f), -45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f));
		XEngine::Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f), 45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f), m_Texture2D, 100.0f, 0.8);
		XEngine::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.5f), 45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f), m_Texture2D, 100.0f, 0.8);
		XEngine::Renderer2D::DrawQuad(glm::vec2(1.0f, -1.0f), glm::vec2(10.0f), 45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f), m_Texture2D, 100.0f, 0.8);
	    
		XEngine::Renderer2D::EndScense();


		XEngine::Renderer2D::BeginScense(m_CameraController.GetCamera());

		for(float y = -5.0f; y < 5.0f; y += 0.5f)
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f ,0.5f};
				XEngine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}

		XEngine::Renderer2D::EndScense();
	}
#endif
}

void Sandbox2D::OnEvent(XEngine::Event& e) {
	m_CameraController.OnEvent(e);

	if (e.GetEventType() == XEngine::EventType::WindowResize) {
		auto& re = (XEngine::WindowResizeEvent&)e;

		float zoom =  static_cast<float>(re.GetWidth());
	}

}

struct ImGuiDemoDockspaceArgs
{
	bool                IsFullscreen = true;
	bool                KeepWindowPadding = false; 
	ImGuiDockNodeFlags  DockSpaceFlags = ImGuiDockNodeFlags_None;
};

static void ShowExampleAppDockSpaceAdvanced(ImGuiDemoDockspaceArgs* args, bool* p_open)
{
	ImGuiDockNodeFlags dockspace_flags = args->DockSpaceFlags;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (args->IsFullscreen)
	{
		// Fullscreen dockspace: practically the same as calling DockSpaceOverViewport();
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;
	}
	else
	{
		// Floating dockspace
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}


	if (!args->KeepWindowPadding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Window with a DockSpace", p_open, window_flags);

	if (!args->KeepWindowPadding)
		ImGui::PopStyleVar();

	if (args->IsFullscreen)
		ImGui::PopStyleVar(2);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();
}

static void ShowExampleAppDockSpaceBasic(ImGuiDockNodeFlags flags)
{

	ImGui::DockSpaceOverViewport(0, nullptr, flags);
}

void Sandbox2D::OnImGuiRender() {
	

	static bool EnableDockspace = true;
	if (EnableDockspace)
	{
		static bool p_open = true;
		static int opt_demo_mode = 0;

		static ImGuiDemoDockspaceArgs args;

		if (opt_demo_mode == 0)
			ShowExampleAppDockSpaceBasic(args.DockSpaceFlags);
		else
			ShowExampleAppDockSpaceAdvanced(&args, &p_open);

		ImGui::Begin("Dock", &p_open, ImGuiWindowFlags_MenuBar);

		// Show demo options and help
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) XEngine::Application::Get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
		ImGui::Begin("Settings");
		auto stats = XEngine::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVetexCount());
		ImGui::Text("Indices: %d", stats.GetTatalIndexCount());
		ImGui::ColorEdit4("Square Color", &m_SquareColor[0]);

		unsigned int texture = m_Framebuffer->GetColorAttachmentID();
		ImTextureRef texRef(texture);
		ImGui::Image(texRef, { 1280, 720 }, ImVec2{ 0,1 }, ImVec2{1, 0});
		ImGui::End();
	
	
}