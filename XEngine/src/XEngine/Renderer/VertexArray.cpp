
#include "xepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace XEngine {
	VertexArray* XEngine::VertexArray::create(){
		switch (Renderer::GetAPI()) {
		case RendererAPI::None: XE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}
		XE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}

}
