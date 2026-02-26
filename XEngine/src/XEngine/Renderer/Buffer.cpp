#include "xepch.h"

#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "XEngine/Renderer/Renderer.h"

namespace XEngine {
	VertexBuffer* VertexBuffer::create(float* vertices, unsigned int size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: XE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}
		XE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create(unsigned int* indices, unsigned int count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: XE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}
		XE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}