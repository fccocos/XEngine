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
	XEngine::Ref<XEngine::Shader> m_Shader;
	XEngine::Ref<XEngine::VertexArray> m_VertexArray;
	// XEngine::Ref<XEngine::Texture2D> m_Texture2D;


	XEngine::OrthographicCameraController m_CameraController;
	

	glm::vec4 m_SquareColor{ 0.2f, 0.3f, 0.8f, 1.0f };
	float m_SquareMoveSpeed = 1.0f;
};

