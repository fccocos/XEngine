#pragma once

#include <random>
#include <glm/glm.hpp>

/*
* 随机数引擎（Engine）
*	std::default_random_engine 默认随机引擎，通常用于简单的随机数生成
*	std::linear_congruential_engine 线性同余引擎，通过线性同余公式生成随机数
*	std::mersenne_twister_engine 美森旋转算法引擎，生成高质量的随机数，性能较好
*	std::sbtract_with_carry_engine: 带进位的减法引擎，合适需要随机性高的场景
* 随机数分布 (Distribution)
*	均匀分布
*		std::uniform_int_distribution: 均匀分布的整数
*		std::uniform_real_distribution: 均匀分布的浮点数
*	伯努利分布
*		std::bernouli_distribution: bool值，表示成功或失败的概率
*	正态分布
*		std::discrete_distribution: 正态分布的浮点数
*	指数分布
*		std::exponential_distribution: 指数分布的浮点数
*	泊松分布
*		std::poisson_distribution: 泊松分布的浮点数
* 随机数种子	(Reed)
*	std::random_device: 一个非确定性随机数生成器
*/

namespace XEngine {

	// 单例
	class Random {
	public:
		static float getRandomNumber(float left, float right) {
			std::random_device rd;
			std::mt19937 m_engine(rd());
			std::uniform_real_distribution<float> distribution(left, right);
			
			return distribution(m_engine);
		}
		static std::vector<float> getRandomNumbers(float left, float right, unsigned int count) {
			std::random_device rd;
			std::mt19937 m_engine(rd());
			std::uniform_real_distribution<float> distribution(left, right);
			std::vector<float> randomNumbers;
			randomNumbers.resize(count);
			for (unsigned int i = 0; i < count; i++) {
				randomNumbers.push_back(distribution(m_engine));
			}
			return randomNumbers;
		}
	private:


		
	};
}