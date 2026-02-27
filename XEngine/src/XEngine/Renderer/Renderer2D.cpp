#include "xepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"
namespace XEngine {

	struct Renderer2DStroge {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStroge* s_Data;

	void  Renderer2D::Init() {
		s_Data = new Renderer2DStroge;
		s_Data->QuadVertexArray = VertexArray::create();
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,// 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 0.0f,// 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 0.0f,// 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f,// 0.0f, 1.0f  // 3
		};
		Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" } ,
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));
		s_Data->QuadVertexArray->setIndexBuffer(squareIB);
		s_Data->FlatColorShader = Shader::Create("Assert/Shaders/flatColorSquare.glsl");
	}

	void Renderer2D::BeginScense(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer2D::EndScense() {

	}
	void Renderer2D::Submit() {

	}

	void Renderer2D::DrawQuard(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuard({ position.x, position.y, 1.0f }, size, color);

	}

	void Renderer2D::DrawQuard(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}
}
