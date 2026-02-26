#pragma once

#include "XEngine/Renderer/RendererAPI.h"

namespace XEngine {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		void Init() override;
		void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}
