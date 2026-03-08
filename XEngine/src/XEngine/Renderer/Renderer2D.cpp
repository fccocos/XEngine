#include "xepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>



namespace XEngine {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float texIndex;
		float TilingFactor;
		float MixFactor;
	};

	// 渲染数据
	struct Render2DData {

		static const unsigned int MaxQuads = 10000; // 最大四边形数量
		static const unsigned int MaxVertecies = MaxQuads * 4; // 最大顶点数量
		static const unsigned int MaxIndicies = MaxQuads * 6; // 最带索引对象数量
		static const unsigned int MaxTextureSlots = 32; // 最大纹理槽数量 TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;   // 顶点数组对象
		Ref<VertexBuffer> QuadVertexBuffer; // 顶点缓冲独享
		Ref<Shader> MixShader;              // 着色器对象
		Ref<Texture2D> WhiteTexture;        // 默认纹理对象

		unsigned int QuadIndexCount = 0;    // 四边形索引计数

		QuadVertex* QuadVerteBufferBase = nullptr; // 四边形顶点缓冲器基指针
		QuadVertex* QuadVertexBufferPtr = nullptr; // 四边形顶点缓冲器游标指针
		
		glm::vec4 QuadVertexPosition[4];
		glm::vec2 QuadTexCoord[4];

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots; // 存放纹理的槽容器
		unsigned int TextureSlotIndex = 0;

		
		
		Renderer2D::Statistics Stats;
	};

	static Render2DData s_Data;

	static float SreachTextureID(const Ref<Texture2D>& texture) {

		// 找到texture对应的textureID
		for (unsigned int i = 0; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				return (float)i;
			}
		}
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
		return s_Data.TextureSlotIndex-1;
	}

	// 渲染初始化：渲染之前数据的初始化
	void  Renderer2D::Init() {
		XE_PROFILE_FUNCTION();
		// vertex array
		s_Data.QuadVertexArray = VertexArray::create();
		s_Data.QuadVertexBuffer = VertexBuffer::create(s_Data.MaxVertecies*sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     } ,
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Float,  "a_MixFactor"    }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		
		// vertex buffer
		s_Data.QuadVerteBufferBase = new QuadVertex[s_Data.MaxVertecies];
		
		// index
		unsigned int* quadIndices = new unsigned int[s_Data.MaxIndicies];

		unsigned int offset = 0;
		for (unsigned int i = 0; i < s_Data.MaxIndicies; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;

		}

		Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_Data.MaxIndicies);
		s_Data.QuadVertexArray->setIndexBuffer(quadIB);
		delete[] quadIndices;

		// default texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		unsigned int whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(unsigned int));

		// shader
		s_Data.MixShader = Shader::Create("Assert/Shaders/TextureAndColorMix.glsl");
		s_Data.MixShader->Bind();

		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}
		s_Data.MixShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// 四边形顶点初始化
		s_Data.QuadVertexPosition[0] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[2] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadTexCoord[0] = { 0.0f, 0.0f };
		s_Data.QuadTexCoord[1] = { 1.0f, 0.0f };
		s_Data.QuadTexCoord[2] = { 1.0f, 1.0f };
		s_Data.QuadTexCoord[3] = { 0.0f, 1.0f };
	}

	void Renderer2D::BeginScense(const Camera& camera, const glm::mat4& transform) {
		XE_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * transform;


		s_Data.MixShader->Bind();
		s_Data.MixShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVerteBufferBase;
		s_Data.TextureSlotIndex = 0;
	}

	void Renderer2D::BeginScense(const OrthographicCamera& camera) {
		XE_PROFILE_FUNCTION();

		s_Data.MixShader->Bind();
		s_Data.MixShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVerteBufferBase;
		s_Data.TextureSlotIndex = 0;


		
	}
	void Renderer2D::EndScense() {
		XE_PROFILE_FUNCTION();

		unsigned int dataSize =(unsigned char* )s_Data.QuadVertexBufferPtr - (unsigned char*)s_Data.QuadVerteBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVerteBufferBase, dataSize);

		Flush();
	}
	void Renderer2D::Submit() {
		XE_PROFILE_FUNCTION();
	}
	void Renderer2D::Shutdown() {
		XE_PROFILE_FUNCTION();
	}

	void Renderer2D::Flush() {
		XE_PROFILE_FUNCTION();
		// Bind Textures
		for (unsigned int i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset() {

		EndScense();
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVerteBufferBase;
		s_Data.TextureSlotIndex = 0;

	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		XE_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 1.0f }, size, 0.0f, color, s_Data.WhiteTexture, 1.0f, 1.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		XE_PROFILE_FUNCTION();

		DrawQuad(position, size, 0.0f, color, s_Data.WhiteTexture, 1.0f, 1.0f);
	
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
		DrawQuad({ position.x, position.y, 1.0f }, size, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, texture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
		XE_PROFILE_FUNCTION();
		DrawQuad(position, size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, texture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 1.0f }, size, angle, color, s_Data.WhiteTexture, 1.0f, 1.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color) {
		XE_PROFILE_FUNCTION();
		DrawQuad(position, size, angle, color, s_Data.WhiteTexture, 1.0f, 1.0f);

		
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture) {
		XE_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 1.0f }, size, angle, { 1.0f, 1.0f, 1.0f, 1.0f }, texture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture) {
		XE_PROFILE_FUNCTION();
		DrawQuad(position, size, angle, { 1.0f, 1.0f, 1.0f, 1.0f }, texture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 1.0f }, size, angle, color, texture, tilingFactor, mixFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();
		
		if (s_Data.QuadIndexCount >= Render2DData::MaxIndicies)
			FlushAndReset();

		float textureID = SreachTextureID(texture);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)               // 平移
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0.0f, 0.0f, 1.0f })   // 旋转
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });                  // 缩放

		for (unsigned int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoord[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
			s_Data.QuadVertexBufferPtr++;
		}
	
		s_Data.QuadIndexCount += 6;

		// 状态设置

		s_Data.Stats.QuadCount++;
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture) {
		XE_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 1.0f }, size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, subTexture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture) {
		XE_PROFILE_FUNCTION();
		DrawQuad(position, size, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, subTexture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<SubTexture2D>& subTexture) {
		XE_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 1.0f }, size, angle, { 1.0f, 1.0f, 1.0f, 1.0f }, subTexture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<SubTexture2D>& subTexture) {
		XE_PROFILE_FUNCTION();
		DrawQuad(position, size, angle, { 1.0f, 1.0f, 1.0f, 1.0f }, subTexture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();
		DrawQuad({ position.x, position.y, 1.0f }, size, angle, color, subTexture, tilingFactor, mixFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Render2DData::MaxIndicies)
			FlushAndReset();

		float textureID = SreachTextureID(subTexture->GetTexture());

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)               // 平移
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0.0f, 0.0f, 1.0f })   // 旋转
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });                  // 缩放

		for (unsigned int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = subTexture->GetTexCoords()[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
			s_Data.QuadVertexBufferPtr++;
		}
		
		s_Data.QuadIndexCount += 6;

		// 状态设置

		s_Data.Stats.QuadCount++;
	}


	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Render2DData::MaxIndicies)
			FlushAndReset();

		float textureID = SreachTextureID(texture);

		for (unsigned int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoord[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// 状态设置
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Render2DData::MaxIndicies)
			FlushAndReset();

		float textureID = SreachTextureID(subTexture->GetTexture());

		for (unsigned int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = subTexture->GetTexCoords()[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// 状态设置

		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();
		if (s_Data.QuadIndexCount >= Render2DData::MaxIndicies)
			FlushAndReset();
		float textureID = SreachTextureID(s_Data.WhiteTexture);
		for (unsigned int i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord =s_Data.QuadTexCoord[i];
			s_Data.QuadVertexBufferPtr->texIndex = textureID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		// 状态设置
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
		DrawQuad(transform, color, 1.0f, 1.0f);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture) {
		DrawQuad(transform, { 1.0f, 1.0f, 1.0f, 1.0f }, texture, 1.0f, 0.0f);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture) {
		DrawQuad(transform, { 1.0f, 1.0f, 1.0f, 1.0f }, subTexture, 1.0f, 0.0f);
	}


	void Renderer2D::DrawPoint(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {

	}
	void Renderer2D::DrawPoint(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {

	}
	void Renderer2D::DrawPoint(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {

	}
	void Renderer2D::DrawPoint(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {

	}
	void Renderer2D::DrawPoint(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color) {

	}
	void Renderer2D::DrawPoint(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color) {

	}
	void Renderer2D::DrawPoint(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture) {

	}
	void Renderer2D::DrawPoint(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture) {

	}
	void Renderer2D::DrawPoint(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor) {

	}
	void Renderer2D::DrawPoint(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, float mixFactor) {
		XE_PROFILE_FUNCTION();
		
		if (s_Data.QuadIndexCount >= Render2DData::MaxIndicies)
			FlushAndReset();

		float textureID = SreachTextureID(texture);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)               // 平移
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0.0f, 0.0f, 1.0f })   // 旋转
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });                  // 缩放

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->texIndex = textureID;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->texIndex = textureID;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->texIndex = textureID;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->texIndex = textureID;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr->MixFactor = mixFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		// 状态设置

		s_Data.Stats.QuadCount++;
	}
	
	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}
	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
	}

}
