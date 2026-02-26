#pragma once
#include "XEngine/Renderer/VertexArray.h"
namespace XEngine {
	class OpenGLVertexArray :public VertexArray {
	public:
		virtual ~OpenGLVertexArray();
		OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override; // 希望换成对象能够引用计数
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const Ref<IndexBuffer>& GetIndexBuffers() const override;
	private:
		unsigned int m_VertexArryID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
