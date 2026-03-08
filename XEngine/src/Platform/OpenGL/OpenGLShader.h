#pragma once

#include "XEngine/Renderer/Shader.h"

namespace XEngine {
	class OpenGLShader:public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		const std::string& GetName() const override { return m_Name; };
		void Bind() const override;
		void Unbind() const override;

		void SetFloat(const std::string& name, float value)             override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetInt(const std::string& name, int value)                 override;
		void SetInt2(const std::string& name, const glm::ivec2& value)  override;
		void SetInt3(const std::string& name, const glm::ivec3& value)  override;
		void SetInt4(const std::string& name, const glm::ivec4& value)  override;
		void SetIntArray(const std::string& name, int* values, unsigned int count) override;
		void SetMat2(const std::string& name, const glm::mat2& value)   override;
		void SetMat3(const std::string& name, const glm::mat3& value)   override;
		void SetMat4(const std::string& name, const glm::mat4& value)   override;
		
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value);
		void UploadUniformInt3(const std::string& name, const glm::ivec3& value);
		void UploadUniformInt4(const std::string& name, const glm::ivec4& value);
		void UploadUniformIntArray(const std::string& name, int* values, unsigned int count);

		void UploadUniformMat2(const std::string& name, const glm::mat2& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
	private:
		void Compile(const std::unordered_map<unsigned int, std::string>& shaderSources);
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);
	private:
		unsigned int m_RendererID = 0;
		std::string m_Name;
	};
}
