#pragma once
#include <XEngine.h>
#include "Panels/SceneHierarchyPanel.h"

#include <imgui.h>
namespace XEngine {
	class EditorLayer :public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(TimeStep timeStep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		EditorLayer();
		virtual ~EditorLayer() = default;
	private:
		Ref<Texture2D> m_Texture2D;
		Ref<Framebuffer> m_Framebuffer;
		OrthographicCameraController m_CameraController;
		
		glm::vec2 m_ViewportSize;

		// event
		bool m_ViewportFoused;
		bool m_ViewportHovered;

		// scene
		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SencondCamera;
		bool isChangeCamera = true;

		// particle
		Scope<ParticleSystemManager> m_ParcticleManager;
		Scope<ParticleSystemManager> m_PureColorParcticleManager;
		Ref<Texture2D> m_RainTexture;
		Ref<ParticleRules> m_RainRules;
		Scope<PointParticleRenderer> m_RainRenderer;
		Ref<Texture2D> m_FireTexture;
		Ref<ParticleRules> m_FireRules;
		Scope<SpriteParticleRenderer> m_FireRenderer;
		Ref<ParticleRules> m_PureColorRules;
		Scope<PointParticleRenderer> m_PureColorRenderer;
		unsigned long long m_counter = 0;

		// spriteAlitas
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TilingRoad;

		// scene hierarchy panel
		SceneHierarchyPanel m_SceneHierarchyPanel;

		// font
		ImFont* m_Font16;
		ImFont* m_Font18;
		ImFont* m_Font20;
		
	};


}

