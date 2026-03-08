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
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;
		XE_CORE_ASSERT(internalFormat & dataFormat, "Format not surppoted!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(unsigned int width, unsigned int height):m_Width(width), m_Height(height) {
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		
		XE_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not surppoted!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);

	}

	bool OpenGLTexture2D::operator==(const Texture& other) const {
		return m_TextureID == dynamic_cast<const OpenGLTexture2D&>(other).m_TextureID;
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

	void OpenGLTexture2D::SetData(void* data, unsigned int size) {
		unsigned int pbc = m_DataFormat == GL_RGBA ? 4 : 3;
		XE_CORE_ASSERT(size == m_Width * m_Height * pbc, "Data mus be entire texture");
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_INT, data);
	}
}