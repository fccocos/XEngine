#include "XEngine/Renderer/Texture.h"

namespace XEngine {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(unsigned int width, unsigned int height);
		virtual ~OpenGLTexture2D();
		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;
		virtual void Bind(unsigned int slot) const override;
		bool operator==(const Texture& other) const override;

		unsigned int GetTextureID() const override { return m_TextureID; }

		void SetData(void* data, unsigned int size) override;
		//virtual void Unbind() const override;
	private:
		std::string m_Path;
		unsigned int m_Width, m_Height;
		unsigned int m_TextureID;
		unsigned int m_InternalFormat, m_DataFormat;
	};
}