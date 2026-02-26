#pragma once

#include "RendererAPI.h"

namespace XEngine {
	class RenderCommand {
	public:
		inline static void Init()  { return s_RendererAPI->Init(); }
		inline static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }
	private:
		static RendererAPI* s_RendererAPI;
	};
}
