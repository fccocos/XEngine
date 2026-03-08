#include "xepch.h"
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>


namespace XEngine {
	
	static const unsigned int s_MaxFramebufferSize = 32;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specificaction):m_Specification(specificaction) {
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void OpenGLFramebuffer::Invalidate() {

		if (m_FramebufferID) {
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
			glDeleteFramebuffers(1, &m_FramebufferID);
		}

		glCreateFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		glGenTextures(1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(m_Specification.Width), static_cast<GLsizei>(m_Specification.Height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		//glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Specification.Width), static_cast<GLsizei>(m_Specification.Height));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Specification.Width), static_cast<GLsizei>(m_Specification.Height), 0,
			GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);


		XE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void OpenGLFramebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
	}
	void OpenGLFramebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const {
		return m_Specification;
	}

	void OpenGLFramebuffer::Resize(const glm::vec2& size) {
		Resize(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
		
	}

	void OpenGLFramebuffer::Resize(unsigned int width, unsigned int height) {

		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {

			LOG_DEBUG_SHORT("Atempted to resize frambuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

		
}