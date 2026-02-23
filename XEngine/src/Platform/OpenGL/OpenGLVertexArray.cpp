#include "xepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace XEngine {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Mat2: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Bool: return GL_BOOL;
		}
		XE_CORE_ASSERT(false, "Unkown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_VertexArryID);
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glGenVertexArrays(1, &m_VertexArryID);
	}
	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_VertexArryID);
	}
	void OpenGLVertexArray::Unbind() const  {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		XE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		glBindVertexArray(m_VertexArryID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		unsigned index = 0;
		for (const auto& ele : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				ele.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(ele.Type),
				ele.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStrides(),
				reinterpret_cast<void*>(ele.Offset));
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_VertexArryID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const {
		return m_VertexBuffers;
	}
	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffers() const {
		return m_IndexBuffer;
	}

}