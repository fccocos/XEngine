#pragma once
#include "XEngine/Renderer/Buffer.h"


namespace XEngine {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		unsigned int m_VertexBufferID;
		BufferLayout m_Layout;
	};


	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int size);
		virtual ~OpenGLIndexBuffer() {}
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual unsigned int GetCount() const override;
	private:
		unsigned int m_IndexBufferID;
		unsigned int m_Count;

	};

}


