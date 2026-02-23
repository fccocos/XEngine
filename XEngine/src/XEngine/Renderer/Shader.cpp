#include "xepch.h"

#include "Shader.h"

#include <glad/glad.h>

namespace XEngine {
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSource = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		int isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled) {
			int maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(vertexShader);
			
			XE_CORE_ERROR("{0}", infoLog.data());
			XE_CORE_ASSERT(false, "Vertex shader compilation failure!");
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled) {
			int maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(fragmentShader);

			XE_CORE_ERROR("{0}", infoLog.data());
			XE_CORE_ASSERT(false, "Fragment shader compilation failure!");
		}

		m_RendererID = glCreateProgram();

		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);
		int isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (!isLinked) {
			int maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(m_RendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			XE_CORE_ERROR("{0}", infoLog.data());
			XE_CORE_ASSERT(false, "Shader porgram compilation failure!");
		}

		// ·ÖÀë×ÅÉ«Æ÷
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);

	}

	Shader::~Shader(){
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const{
		glUseProgram(0);
	}
}