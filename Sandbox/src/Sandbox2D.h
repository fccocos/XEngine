#pragma once
#include <XEngine.h>


class Sandbox2D:public XEngine::Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(XEngine::TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(XEngine::Event& e) override;
	Sandbox2D();
	virtual ~Sandbox2D() = default;
private:
	// temp
	XEngine::Ref<XEngine::Shader> m_Shader, m_TextureShader;
	XEngine::Ref<XEngine::VertexArray> m_VertexArray;
	XEngine::Ref<XEngine::Texture2D> m_Texture2D;
	XEngine::Ref<XEngine::Framebuffer> m_Framebuffer;

	XEngine::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor{ 0.2f, 0.3f, 0.8f, 1.0f };
	float m_SquareMoveSpeed = 1.0f;


	// particle
	XEngine::Scope<XEngine::ParticleSystemManager> m_ParcticleManager;
	XEngine::Scope<XEngine::ParticleSystemManager> m_PureColorParcticleManager;

	XEngine::Ref<XEngine::Texture2D> m_RainTexture;
	XEngine::Ref<XEngine::ParticleRules> m_RainRules;
	XEngine::Scope<XEngine::PointParticleRenderer> m_RainRenderer;

	XEngine::Ref<XEngine::Texture2D> m_FireTexture;
	XEngine::Ref<XEngine::ParticleRules> m_FireRules;
	XEngine::Scope<XEngine::SpriteParticleRenderer> m_FireRenderer;

	XEngine::Ref<XEngine::ParticleRules> m_PureColorRules;
	XEngine::Scope<XEngine::PointParticleRenderer> m_PureColorRenderer;

	// spriteAlitas
	XEngine::Ref<XEngine::Texture2D> m_SpriteSheet;
	XEngine::Ref<XEngine::SubTexture2D> m_TilingRoad;

	unsigned long long m_counter = 0;
};

