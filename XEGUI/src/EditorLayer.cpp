#include "EditorLayer.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "XEngine/Renderer/RenderCommand.h"

#include "XEngine/Scene/ScriptableEnttity.h"

// #define TEST1 0

namespace XEngine {

	class CameraController :public ScriptableEntity {
	public:
		void OnCreate() override{
			
		}

		void OnDestroy()override {

		}

		void OnUpdate(TimeStep ts){
			auto& transform = GetComponent<TransformComponent>().GetTransform();
			float speed = 5.0f;
			transform[3][0] = rand() % 5 - 2.50f;
			if (Input::IsKeyPressed(XE_KEY_A))
				transform[3][0] -= speed * ts;
			if (Input::IsKeyPressed(XE_KEY_D))
				transform[3][0] += speed * ts;
			if (Input::IsKeyPressed(XE_KEY_W))
				transform[3][1] -= speed * ts;
			if (Input::IsKeyPressed(XE_KEY_S))
				transform[3][1] += speed * ts;
		}
	};

	EditorLayer::EditorLayer() :
		Layer("Example"),
		m_CameraController(3840.0f / 2160.0f, true),
		m_ViewportFoused(false),
		m_ViewportHovered(false),
		m_ViewportSize(0.0f, 0.0f) {

	}

	void EditorLayer::OnAttach() {
		m_Texture2D = Texture2D::Create("Assert/Textures/container.jpg");
		m_SpriteSheet = Texture2D::Create("Assert/Games/Textures/tilemap_packed.png");
		m_TilingRoad = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0.0f,1.0f }, { 16.0f,16.0f }, { 3.0f, 3.0f });

		m_CameraController.GetCamera().SetRotation(90.0f);

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

#if TEST1
		m_Texture2D = Texture2D::Create("Assert/Textures/container.jpg");
		m_FireTexture = Texture2D::Create("Assert/Textures/rain.png");

		m_RainRules = std::make_shared<ParticleRules>(ParticlePresets::rain());
		m_FireRules = std::make_shared<ParticleRules>(ParticlePresets::fire());
		m_PureColorRules = std::make_shared<ParticleRules>(ParticlePresets::pureColor());

		m_FireRenderer = std::make_unique<SpriteParticleRenderer>(m_FireTexture);
		m_RainRenderer = std::make_unique<PointParticleRenderer>();
		m_PureColorRenderer = std::make_unique<PointParticleRenderer>();


		m_ParcticleManager = std::make_unique<ParticleSystemManager>(m_RainRules->m_maxParticles, m_RainRules, std::move(m_RainRenderer));
		m_PureColorParcticleManager = std::make_unique<ParticleSystemManager>(m_PureColorRules->m_maxParticles, m_PureColorRules, std::move(m_PureColorRenderer));

		m_ParcticleManager->start();
		m_PureColorParcticleManager->start();

#endif

		// ECS
		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity("green square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		m_SquareEntity.AddComponent<PaletteComponent>(glm::vec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f));
		m_SquareEntity = m_ActiveScene->CreateEntity("red square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		m_SquareEntity.AddComponent<PaletteComponent>(glm::vec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SencondCamera = m_ActiveScene->CreateEntity("second Camera Entity");
		auto& cc = m_SencondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		// native script
		m_SencondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		// scene hierarchy panel
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		// font
		ImFontConfig fc;
		fc.MergeMode = false;
		fc.PixelSnapH = true;
		fc.GlyphOffset = ImVec2(0, 0);
		ImGuiIO& io = ImGui::GetIO();
	}

	void EditorLayer::OnDetach() {
#if TEST1
		m_ParcticleManager->stop();
#endif

	}

	void EditorLayer::OnUpdate(TimeStep timeStep) {
		XE_PROFILE_FUNCTION();
		// Resize
		{
			XE_PROFILE_SCOPE("First Frame::Viewport Resize");
			if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
				m_Framebuffer->Resize(static_cast<unsigned int>(m_ViewportSize.x), static_cast<unsigned int>(m_ViewportSize.y));
				m_CameraController.OnResizeOrthographicCameraBounds(m_ViewportSize.x, m_ViewportSize.y);

				m_ActiveScene->OnViewportResize(static_cast<unsigned int>(m_ViewportSize.x), static_cast<unsigned int>(m_ViewportSize.y));
			}
		}
		


		// update
		{
			XE_PROFILE_SCOPE("CameraController::OnUpdate");
			if(m_ViewportFoused)
				m_CameraController.OnUpdate(timeStep);
		}

		Renderer2D::ResetStats();
		{
			XE_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

		}
#if TEST1
		// spirteAlitas
		{
			XE_PROFILE_SCOPE("SpriteAlitas Renderer Draw");
			Renderer2D::BeginScense(m_CameraController.GetCamera());

			Renderer2D::DrawQuad(glm::vec2(0.0f, 0.8f), glm::vec2(0.1f), m_TilingRoad);

			Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f), m_SpriteSheet);

			Renderer2D::EndScense();
			
		}


		// Particle System
		{
			XE_PROFILE_SCOPE("Particle System  Renderer Draw");

			Renderer2D::BeginScense(m_CameraController.GetCamera());

			if (m_counter == 1000) {
				m_ParcticleManager->udpateRules(m_FireRules);
				m_ParcticleManager->setRenderer(std::move(m_FireRenderer));
				m_ParcticleManager->burst(100);
			}
			m_counter++;
			m_ParcticleManager->update(timeStep);

			if (Input::IsMouseButtonPressed(XE_MOUSE_BUTTON_LEFT)) {
				auto [x, y] = Input::GetMousePosition();
				auto width = Application::Get().GetWindow().GetWidth();
				auto height = Application::Get().GetWindow().GetHeight();

				auto bounds = m_CameraController.GetOrthographicCameraBounds();
				auto pos = m_CameraController.GetCamera().GetPosition();

				x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
				y = bounds.GetHeight() * 0.5 - (y / height) * bounds.GetHeight();

				m_PureColorRules->m_emitPosition = { x, y, 0.0f };

				m_PureColorParcticleManager->udpateRules(m_PureColorRules);
				m_PureColorParcticleManager->update(timeStep);

			}

			Renderer2D::EndScense();
		}



		{
			XE_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScense(m_CameraController.GetCamera());

			Renderer2D::DrawQuad(glm::vec2(0.5f), glm::vec2(0.5), glm::vec4(0.8, 0.2, 0.3, 1.0f));
			Renderer2D::DrawQuad(glm::vec2(-0.5f, 0.5f), glm::vec2(0.5f), glm::vec4(0.3, 0.2, 0.8, 1.0f));
			Renderer2D::DrawQuad(glm::vec2(0.0f, 0.8f), glm::vec2(0.8f), glm::vec4(0.3, 0.8, 0.2, 1.0f));
			Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(0.5f), m_Texture2D);

			Renderer2D::DrawQuad(glm::vec2(0.0f, 0.8f), glm::vec2(0.8f), -45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f));
			Renderer2D::DrawQuad(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f), 45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f), m_Texture2D, 100.0f, 0.8);
			Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(1.5f), 45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f), m_Texture2D, 100.0f, 0.8);
			Renderer2D::DrawQuad(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f), 45.0f, glm::vec4(0.3, 0.8, 0.2, 1.0f), m_Texture2D, 100.0f, 0.8);

			Renderer2D::EndScense();


			Renderer2D::BeginScense(m_CameraController.GetCamera());

			for (float y = -5.0f; y < 5.0f; y += 0.5f)
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f ,0.5f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}

			Renderer2D::EndScense();
		}
#endif
		// Scene Renderer Update
		{
			XE_PROFILE_SCOPE("Scene Renderer Udpate");
			m_ActiveScene->OnUpdate(timeStep);

		}

	

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e) {
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == EventType::WindowResize) {
			auto& re = (WindowResizeEvent&)e;

			float zoom = static_cast<float>(re.GetWidth());
		}

	}

	void EditorLayer::OnImGuiRender() {

		static bool p_open = true;
		

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize =  style.WindowMinSize.x;
		style.WindowMinSize.x = 370;
		
		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar()){
			if (ImGui::BeginMenu("File")){
				if (ImGui::MenuItem("Exit", "Esc")) { XEngine::Application::Get().close(); }
				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}
		
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport", &p_open);
			ImGui::PopStyleVar();
			
			m_ViewportFoused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlocEvents(!m_ViewportFoused || !m_ViewportHovered);
			
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				m_Framebuffer->Resize(m_ViewportSize);
				m_CameraController.OnResizeOrthographicCameraBounds(m_ViewportSize);
			}
			unsigned int texture = m_Framebuffer->GetColorAttachmentID();
			ImTextureRef texRef(texture);
			ImGui::Image(texRef, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::Begin("2D Scene Render"); 
		{
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Statics:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVetexCount());
			ImGui::Text("Indices: %d", stats.GetTatalIndexCount());
		}
		{
			ImGui::ShowDemoWindow();
		}
		ImGui::End();

		// 层级面板渲染
		m_SceneHierarchyPanel.OnImGuiRender();
			
	}

}