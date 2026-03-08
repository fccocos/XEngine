#pragma once
#include <glm/glm.hpp>

namespace XEngine {
	struct FramebufferSpecification {
		unsigned int Width=1280, Height=720;
		//FrameBufferFromat Fromat;
		unsigned int Samples = 1;

		bool SwapChainTarget = false; // 解除所有的帧缓冲区

	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(const glm::vec2& size) = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;
		virtual unsigned int GetColorAttachmentID() const = 0;
		virtual unsigned int GetDepthAttachmentID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}

