#pragma once
#include "XEngine/Core/log.h"

namespace XEngine {

	enum class ShaderDataType: unsigned char {
		None = 0,Float,Float2,Float3,Float4,Mat2, Mat3,Mat4,Int,Int2,Int3,Int4,Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Int:
		case ShaderDataType::Float: return 4;

		case ShaderDataType::Int2:
		case ShaderDataType::Float2: return 4 * 2;

		case ShaderDataType::Int3:
		case ShaderDataType::Float3: return 4 * 3;

		case ShaderDataType::Int4:
		case ShaderDataType::Float4: return 4 * 4;

		case ShaderDataType::Mat2: return 4 * 2 * 2;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Bool: return 1;
		}
		XE_CORE_ASSERT(false, "Unkown ShaderDataType!");
		return 0;
	}

	struct BufferElement {
	public:
		std::string Name;
		unsigned int Offset;
		unsigned int Size;
		ShaderDataType Type;
		bool Normalized;
	public:
		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized=false) :Type(type), Name(name),  Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		unsigned int GetComponentCount() const {
			switch (Type) {
				case ShaderDataType::Int:
				case ShaderDataType::Float: return 1;

				case ShaderDataType::Int2:
				case ShaderDataType::Float2: return  2;

				case ShaderDataType::Int3:
				case ShaderDataType::Float3: return 3;

				case ShaderDataType::Int4:
				case ShaderDataType::Float4: return 4;

				case ShaderDataType::Mat2: return 2*2;
				case ShaderDataType::Mat3: return 3*3;
				case ShaderDataType::Mat4: return 4*4;
				case ShaderDataType::Bool: return 1;
			}
			XE_CORE_ASSERT(false, "Unkown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& element) : m_Elements(element) {
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStrides() const { return m_Stride; }

		// 添加const迭代器支持以修复E1087错误
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator cbegin() const { return m_Elements.cbegin(); }
		std::vector<BufferElement>::const_iterator cend() const { return m_Elements.cend(); }
	private:
		void CalculateOffsetsAndStride() {
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& ele : m_Elements) {
				ele.Offset = offset;
				offset += ele.Size;
				m_Stride += ele.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> create(float* vertices, unsigned int size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static Ref<IndexBuffer> create(unsigned int* indices, unsigned int count);
	};
}
