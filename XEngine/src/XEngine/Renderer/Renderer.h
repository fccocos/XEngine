#pragma once

namespace XEngine {
	enum class RendererAPI {
		None = 0,
		OpenGL,
		Vulkan,
		XD11,
		XD12
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}
