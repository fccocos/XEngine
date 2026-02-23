#pragma once
#include "XEngine/Renderer/VertexArray.h"
namespace XEngine {
	class OpenGLVertexArray :public VertexArray {
	public:
		virtual ~OpenGLVertexArray();
		OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override; // 希望换成对象能够引用计数
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffers() const override;
	private:
		unsigned int m_VertexArryID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}
