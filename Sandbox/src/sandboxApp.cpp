#include <xepch.h>
#include <XEngine.h>
#include <XEngine/Core/EntryPoint.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox2D.h"

#include "imgui.h"

class ExampleLayer : public XEngine::Layer {
public:
	ExampleLayer() :Layer("Example"), m_CameraController(3840.0f/2160.0f, true), m_CameraPosition(0.0f),m_SquarePosition(0.0f) {
		// Vertex Array
		m_VertexArray = XEngine::VertexArray::create();
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		// 创建 vertexBuffer
		std::shared_ptr<XEngine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = XEngine::VertexBuffer::create(vertices, sizeof(vertices));
		// 顶点布局
		XEngine::BufferLayout layout = {
			{XEngine::ShaderDataType::Float3, "a_Position"},
			{XEngine::ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(layout); // 设置顶点布局
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// 将VertexBuffer添加到VertexArray
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		// 创建 IndexBuffer
		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<XEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = XEngine::IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int));
		m_VertexArray->setIndexBuffer(m_IndexBuffer);

		// 创建 VAO
		m_SquareVA = XEngine::VertexArray::create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // 3
		};
		// 创建VBO
		std::shared_ptr<XEngine::VertexBuffer> squareVB;
		squareVB= XEngine::VertexBuffer::create(squareVertices, sizeof(squareVertices));
		// 顶点布局
		squareVB->SetLayout({ 
			{ XEngine::ShaderDataType::Float3, "a_Position" } ,
			{ XEngine::ShaderDataType::Float2, "a_TexCoord"}
			});
		// 为SquareVA添加Vertex Buffer
		m_SquareVA->AddVertexBuffer(squareVB);
		// 创建 IndexBuffer
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<XEngine::IndexBuffer> squareIB;
		squareIB = XEngine::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int));
		// 添加 IBO
		m_SquareVA->setIndexBuffer(squareIB);


		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Tranform;
			void main(){
				//gl_Position = vec4(a_Position, 1.0f);
				gl_Position = u_Tranform * u_ViewProjection * vec4(a_Position, 1.0f);
				v_Color = a_Color;
			}				
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			in vec4 v_Color;
			void main(){
				FragColor = v_Color;
			}
		)";
		m_Shader = XEngine::Shader::Create("vertexPosColor", vertexSrc, fragmentSrc);

		// 纹理着色器
		std::string textureVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			out vec3 v_Position;
			out vec2 v_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main(){
				gl_Position = u_Transform * u_ViewProjection * vec4(a_Position, 1.0f);
				v_TexCoord = a_TexCoord;
			}				
		)";
		std::string textureFragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			in vec2 v_TexCoord;
			uniform sampler2D u_Texture;
			void main(){
				FragColor =texture(u_Texture, v_TexCoord);
			}
		)";
		auto textureShader = m_ShaderLibrary.Load("Assert/Shaders/Texture.glsl");
		m_Texture2D = XEngine::Texture2D::Create("Assert/Textures/container.jpg");
		m_FaceTexture2D = XEngine::Texture2D::Create("Assert/Textures/awesomeface.png");

		// 着色器
		std::string squareVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			void main(){
				//gl_Position = vec4(a_Position, 1.0f);
				gl_Position = u_Transform * u_ViewProjection * vec4(a_Position, 1.0f);
			}
		)";
		std::string squareFragmentSrc = R"(
			#version 330 core
			out vec4 color;
			uniform vec3 u_Color;
			void main(){
				color = vec4(u_Color, 1.0f);
			}
		)";
		m_SquareShader = XEngine::Shader::Create("SquarePureColorShader", squareVertexSrc, squareFragmentSrc);

	}
	void OnUpdate(XEngine::TimeStep ts) override {
		
		m_CameraController.OnUpdate(ts);

		XEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		XEngine::RenderCommand::Clear();


		XEngine::Renderer::BeginScense(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<XEngine::OpenGLShader>(m_SquareShader)->Bind();
		std::dynamic_pointer_cast<XEngine::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x*0.1f, y * 0.15f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				XEngine::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
			}
		}
		
		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture2D->Bind();
		XEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_FaceTexture2D->Bind();
		XEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		
		
		// Triangle
		//XEngine::Renderer::Submit(m_Shader, m_VertexArray);

		XEngine::Renderer::EndScense();
	}
	void OnEvent(XEngine::Event& e) override {
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == XEngine::EventType::WindowResize) {
			auto& re = (XEngine::WindowResizeEvent&)e;

			float zoom = re.GetWidth();
		}

	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", &m_SquareColor[0]);
		ImGui::End();
	}


private:
	XEngine::ShaderLibrary m_ShaderLibrary;

	XEngine::Ref<XEngine::Shader> m_Shader;
	XEngine::Ref<XEngine::VertexArray> m_VertexArray;

	XEngine::Ref<XEngine::Shader> m_SquareShader;
	XEngine::Ref<XEngine::VertexArray> m_SquareVA;

	XEngine::OrthographicCameraController m_CameraController;

	XEngine::Ref<XEngine::Texture2D> m_Texture2D;
	XEngine::Ref<XEngine::Texture2D> m_FaceTexture2D;

	glm::vec3 m_CameraPosition;
	float m_CameraTranslateSpeed = 1.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRoationSpeed = 30.0f;

	glm::vec3 m_SquarePosition;
	glm::mat4 m_SquareTranform;
	glm::vec3 m_SquareColor{0.2f, 0.3f, 0.8f};
	float m_SquareMoveSpeed = 1.0f;
};

class Sandbox :public XEngine::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};



XEngine::Application* XEngine::CreateApplication() {
	return new Sandbox();
}