#pragma once
#include "XEngine/Renderer/OrthographicCamera.h"

namespace XEngine {
	class Renderer2D
	{
	public:
		static void Init();
		static void BeginScense(const OrthographicCamera& camera);
		static void EndScense();
		static void Submit();
		static void DrawQuard(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuard(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void Shutdown();
	private:
	};

}

