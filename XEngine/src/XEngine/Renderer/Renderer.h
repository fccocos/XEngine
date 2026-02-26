#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace XEngine {
	
	class Renderer {
	public:

		static void Init();
		static void OnWindowResize(unsigned int width, unsigned int height);
		static void BeginScense(const OrthographicCamera& camera);
		static void EndScense();

		static void Submit(const Ref<Shader>& Shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return s_RendererAPI; }
	private:
		struct ScenseData {
			glm::mat4 ViewProjectionMatrix;
		};
		static ScenseData* m_ScenseData;
		static RendererAPI::API s_RendererAPI;
	};
}
