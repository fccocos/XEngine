#include "xepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace XEngine {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :m_Path{path} {
		int width{}, height{}, channels{};
		stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		XE_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		XE_CORE_ASSERT(internalFormat & dataFormat, "Format not surppoted!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_TextureID);
	}

	unsigned int OpenGLTexture2D::GetWidth() const {
		return m_Width;
	}
	unsigned int OpenGLTexture2D::GetHeight() const {
		return m_Height;
	}
	void OpenGLTexture2D::Bind(unsigned int slot=0) const {
		glBindTextureUnit(slot, m_TextureID);
	}
	/*void OpenGLTexture2D::Unbind() const {

	}*/
}