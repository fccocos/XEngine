#include "xepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace XEngine {
	RendererAPI::API Renderer::s_RendererAPI = RendererAPI::API::OpenGL;
	Renderer::ScenseData* Renderer::m_ScenseData = new Renderer::ScenseData;

	void Renderer::Init() {
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}
	
	void Renderer::BeginScense(const OrthographicCamera& camera) {
		m_ScenseData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScense() {

	}

	void Renderer::Submit(const Ref<Shader>& Shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		vertexArray->Bind();
		Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(Shader)->UploadUniformMat4("u_ViewProjection", m_ScenseData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(Shader)->UploadUniformMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(vertexArray);
	}
}