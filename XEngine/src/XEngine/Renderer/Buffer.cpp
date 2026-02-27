#include "xepch.h"

#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "XEngine/Renderer/Renderer.h"

namespace XEngine {
	Ref<VertexBuffer> VertexBuffer::create(float* vertices, unsigned int size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: XE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		XE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(unsigned int* indices, unsigned int count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: XE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}
		XE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}