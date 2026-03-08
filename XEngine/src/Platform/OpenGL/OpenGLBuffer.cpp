#include "xepch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>
namespace XEngine {
	/// /////////////////////////
	/// OpenGLVertexBuffer
	/////////////////////////////
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size){
		XE_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size) {
		XE_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		XE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void OpenGLVertexBuffer::Bind() const{
		XE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		XE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void OpenGLVertexBuffer::SetData(void* data, unsigned int size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/// /////////////////////////////////
	/// OpenGLIndexBuffer
	////////////////////////////////////
	
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count):m_IndexBufferID(0),m_Count(count) {
		XE_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count) :m_IndexBufferID(0), m_Count(count) {
		XE_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind() const {
		XE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		XE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int OpenGLIndexBuffer::GetCount() const {
		return m_Count;
	}

}
