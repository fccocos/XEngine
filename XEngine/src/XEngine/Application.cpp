#include "xepch.h"

#include "Application.h"

#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/log.h"
#include "spdlog/fmt/ostr.h"

#include <glad/glad.h>

#include "XEngine/Input.h"

#include "XEngine/Renderer/Buffer.h"

namespace XEngine {

	Application* Application::s_Instance = nullptr;

// 事件绑定宏
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1) 


	Application::Application() {
		XE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		


		// Vertex Array
		m_VertexArray.reset(VertexArray::create());
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		// 创建 vertexBuffer
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		// 顶点布局
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(layout); // 设置顶点布局
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		
		// 将VertexBuffer添加到VertexArray
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		// 创建 IndexBuffer
		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->setIndexBuffer(m_IndexBuffer);

		// 创建 VAO
		m_SquareVA.reset(VertexArray::create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f, // 0
			 0.75f, -0.75f, 0.0f, // 1
			 0.75f,  0.75f, 0.0f, // 2
			-0.75f,  0.75f, 0.0f  // 3
		};
		// 创建VBO
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		// 顶点布局
		squareVB->SetLayout({ { ShaderDataType::Float3, "a_Position" } });
		// 为SquareVA添加Vertex Buffer
		m_SquareVA->AddVertexBuffer(squareVB);
		// 创建 IndexBuffer
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		// 添加 IBO
		m_SquareVA->setIndexBuffer(squareIB);


		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			void main(){
				gl_Position = vec4(a_Position, 1.0f);
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
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		
		// 着色器
		std::string squareVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			void main(){
				gl_Position = vec4(a_Position, 1.0f);
			}
		)";
		std::string squareFragmentSrc = R"(
			#version 330 core
			out vec4 color;
			void main(){
				color = vec4(0.2f, 0.3f, 0.8f, 0.0f);
			}
		)";
		m_SquareShader.reset(new Shader(squareVertexSrc, squareFragmentSrc));
		
	}

	Application::~Application() {
	
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		// 用事件分发器分发事件
		EventDispather dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		XE_CORE_INFO("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled()) {
				break;
			}
		}

	}

	void Application::run() {
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_SquareShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);
			

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);

			

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}