#pragma once
#include <glm/glm.hpp>

namespace XEngine {
	
	class Math {
	public:
		// 浮点数线性插值（适用于大小、速度等数值）
		static float Lerp(float a, float b, float t) {
			// 限制t的范围在0~1，避免插值超出预期（比如粒子生命周期计算异常）
			t = Clamp(t, 0.0f, 1.0f);
			return a + (b - a) * t;
		}

		// 辅助：将值限制在 min ~ max 之间
		static float Clamp(float value, float min, float max) {
			if (value < min) return min;
			if (value > max) return max;
			return value;
		}

		// 扩展：颜色插值（粒子颜色渐变也能用）
		static const glm::vec4& Lerp(const glm::vec4& a, const glm::vec4& b, float t) {
			t = Clamp(t, 0.0f, 1.0f);
			return glm::vec4(
				a.r + (b.r - a.r) * t,
				a.g + (b.g - a.g) * t,
				a.b + (b.b - a.b) * t,
				a.a + (b.a - a.a) * t
			);
		}

	};
}
