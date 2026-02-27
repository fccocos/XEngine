#include "xepch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace XEngine {

	static unsigned int ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment") {
			return GL_FRAGMENT_SHADER;
		}

		XE_CORE_ASSERT(false, "Unkown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) {
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//         lastSlash   lastDot;
		//               \|/  \|/  
		// Assert/shaders/Teture.glsl

		// Extract name from filepath
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		// 计算截取长度
		auto count = (lastDot == std::string::npos || lastDot < lastSlash)
			? filePath.size() - lastSlash  // 无扩展名 或 . 在目录中（如 "/dir.name/file"）
			: lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);



	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :m_Name(name) {
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath) {
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
			
		}else {
			XE_CORE_ERROR("Could not open file '{0}'", filePath);
		}
		return result;
	}

	std::unordered_map<unsigned int, std::string> OpenGLShader::PreProcess(const std::string& source) {
		std::unordered_map<unsigned int, std::string> shaderSources;
		
		// 获取类型标志: vertex fragment pixel
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			// 从 #type中获取vertex fragment
			size_t eol = source.find_first_of("\r\n", pos);
			XE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			//XE_CORE_TRAC(type);
			XE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specifier");

			// 获取 #type vertex 后面的glsl源码
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos == std::string::npos ? std::string::npos : pos - nextLinePos);
			//XE_CORE_INFO("\n{0}",shaderSources[ShaderTypeFromString(type)]);
		}

		return shaderSources;
	}


	void OpenGLShader::Compile(const std::unordered_map<unsigned int, std::string>& shaderSources) {

		m_RendererID = glCreateProgram();
		std::vector<unsigned int> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());
		for (auto& kv : shaderSources) {
			unsigned int type = kv.first;
			const std::string& source = kv.second;
			unsigned int shader = glCreateShader(type);
			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, nullptr);
			glCompileShader(shader);
			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (!isCompiled) {
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);

				XE_CORE_ERROR("{0}", infoLog.data());
				XE_CORE_ASSERT(false, "Vertex shader compilation failure!");
				break;
			}
			glAttachShader(m_RendererID, shader);
			glShaderIDs.push_back(shader);
			
		}

		glLinkProgram(m_RendererID);
		int isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (!isLinked) {
			int maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(m_RendererID);

			for (auto id : glShaderIDs)
				glDeleteShader(id);
			
			XE_CORE_ERROR("{0}", infoLog.data());
			XE_CORE_ASSERT(false, "Shader porgram compilation failure!");
		}

		// 分离着色器
		for (auto id : glShaderIDs)
			glDetachShader(m_RendererID, id);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2i(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3i(location, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value) {
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}