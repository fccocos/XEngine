#include "xepch.h"
#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "XEngine/Renderer/Renderer.h"

namespace XEngine {
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: XE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
		}
		XE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}
