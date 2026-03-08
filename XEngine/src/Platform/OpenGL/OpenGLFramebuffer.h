#pragma once
#include "XEngine/Renderer/Framebuffer.h"

namespace XEngine {
	class OpenGLFramebuffer:public Framebuffer
	{
	public:
		~OpenGLFramebuffer();
		OpenGLFramebuffer(const FramebufferSpecification& specificaction);

		void Bind() override;
		void Unbind() override;
		unsigned int GetColorAttachmentID() const override { return m_ColorAttachment; }
		unsigned int GetDepthAttachmentID() const override { return m_DepthAttachment; }

		void Resize(const glm::vec2& size) override;
		void Resize(unsigned int width, unsigned int height) override;
		const FramebufferSpecification& GetSpecification() const override;

		void Invalidate();
	private:
		unsigned int m_FramebufferID;
		unsigned int m_ColorAttachment;
		unsigned int m_DepthAttachment;
		FramebufferSpecification m_Specification;

	};
}


