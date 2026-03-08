#pragma once
#include "XEngine/Renderer/OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "XEngine/Renderer/Camera.h"
namespace XEngine {
	class Renderer2D
	{
	public:
		static void Init();
		static void BeginScense(const Camera& camera, const glm::mat4& transform);
		static void BeginScense(const OrthographicCamera& camera);
		static void EndScense();
		static void Submit();
		static void Shutdown();
		static void Flush();


		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor);
		

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<SubTexture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<SubTexture2D>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor, float mixFactor);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor, float mixFactor);
		
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor, float mixFactor);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, float tilingFactor, float mixFactor);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& texture);
		

		static void DrawPoint(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawPoint(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawPoint(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawPoint(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawPoint(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawPoint(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawPoint(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);
		static void DrawPoint(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);
		static void DrawPoint(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor);
		static void DrawPoint(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor);


		// temp
		struct Statistics {
			unsigned int DrawCalls = 0;
			unsigned int QuadCount = 0;

			unsigned int GetTotalVetexCount() { return QuadCount * 4; }
			unsigned int GetTatalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();
	
	private:

		static void FlushAndReset();
	};

}

