#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D():
	Layer("Example"), 
	m_CameraController(3840.0f / 2160.0f, true) 
	
{
}

void Sandbox2D::OnAttach() {
	m_VertexArray = XEngine::VertexArray::create();
	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,// 0.0f, 0.0f, // 0
		 0.5f, -0.5f, 0.0f,// 1.0f, 0.0f, // 1
		 0.5f,  0.5f, 0.0f,// 1.0f, 1.0f, // 2
		-0.5f,  0.5f, 0.0f,// 0.0f, 1.0f  // 3
	};
	XEngine::Ref<XEngine::VertexBuffer> squareVB = XEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ XEngine::ShaderDataType::Float3, "a_Position" } ,
	});
	m_VertexArray->AddVertexBuffer(squareVB);
	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	XEngine::Ref<XEngine::IndexBuffer> squareIB = XEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));
	m_VertexArray->setIndexBuffer(squareIB);
	m_Shader = XEngine::Shader::Create("Assert/Shaders/flatColorSquare.glsl");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(XEngine::TimeStep timeStep) {

	m_CameraController.OnUpdate(timeStep);

	XEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	XEngine::RenderCommand::Clear();

	// TODO: Add these functions - Shader::SetMat4() Shader::SetFloat4()
	XEngine::Renderer2D::BeginScense(m_CameraController.GetCamera());
	XEngine::Renderer2D::DrawQuard(glm::vec3(0.0f), glm::vec2(100, 200), glm::vec4(0.8, 0.2, 0.3, 1.0f));
	XEngine::Renderer2D::EndScense();
}

void Sandbox2D::OnEvent(XEngine::Event& e) {
	m_CameraController.OnEvent(e);

	if (e.GetEventType() == XEngine::EventType::WindowResize) {
		auto& re = (XEngine::WindowResizeEvent&)e;

		float zoom = re.GetWidth();
	}

}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", &m_SquareColor[0]);
	ImGui::End();
}