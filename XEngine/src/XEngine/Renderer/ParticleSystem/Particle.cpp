#include "xepch.h"
#include "Particle.h"
#include "Random.h"

#include "XEngine/Core/Utils.h"

#include <glm/gtc/matrix_transform.hpp>

#include <XEngine/Renderer/Renderer2D.h>

namespace XEngine {
	void Particle::reset() {
		position = { 0.0f, 0.0f, 0.0f };         // 世界坐标
		velocity = { 0.0f, 0.0f, 0.0f };         // 速度向量
		acceleration = { 0.0f, 0.0f, 0.0f };     // 加速度 (F = ma)
		lifetime = 0.0f;                           // 总生命周期(s)
		remainingTime = 0.0f;                      // 剩余生命周期(s)
		size = 0.0f;                               // 粒子大小
		color = { 1.0f, 1.0f, 1.0f, 1.0f };     // 粒子颜色
		rotation = 0.0f;                           // 旋转角度

		// 状态标志
		isActive = false;                           // 是否激活(参与更新/渲染)
	}


	Particle* ParticlePool::getFreeParticle() {
		
		for (auto& particle : m_particles) {
			if (!particle.isActive) {
				particle.isActive = true;
				return &particle;
			}
		}

		return nullptr;
	}

	void ParticlePool::releaseParticle(Particle* particle) {
		if (particle == nullptr) return;

		Particle* poolStart = &m_particles[0];
		Particle* poolEnd = &m_particles[m_particles.size() - 1];
		if (particle < poolStart || particle > poolEnd) return;

		if (particle->isActive) particle->reset();
	
	}


	ParticlePool& ParticlePool::getParticlePoolInstance(unsigned int maxcount) {
		static ParticlePool s_instance(maxcount);
		return s_instance;
	}

	ParticlePool::ParticlePool(unsigned int maxcount) :m_maxCount{ maxcount } {
		m_particles.resize(maxcount);
		unsigned int ID = 0;
		for (auto& particle : m_particles) {
			particle.ID = ID++;
			particle.reset();
		}
	}

	std::vector<Particle*> ParticlePool::getAcitiveParticles() {
		std::vector<Particle*> activeParticles;
		for (auto& particle : m_particles) {
			if (particle.isActive) {
				activeParticles.push_back(&particle);
			}
		}

		return activeParticles;
	}

	void ParticlePool::clear() {
		for (auto& particle : m_particles)
			particle.reset();
	}

	// 生成粒子的随机初始属性(基于规则)
	void ParticleController::generateRandomParicleProps(Particle* particle) {
			XE_CORE_ASSERT(particle, "paritcle is not exsist!");

			// 随机位置(发射原点+范围)
			particle->position = m_rules->m_emitPosition + Random::getRandomNumber(-m_rules->m_emitRange, m_rules->m_emitRange);

			/// 随机方向(基础方向+偏差)
			glm::mat4 transform = glm::rotate(glm::mat4(1.0), glm::radians(Random::getRandomNumber(-m_rules->m_directionVariance, m_rules->m_directionVariance)), { 0.0f, 0.0f, 1.0f });
			glm::vec4 tuple = glm::vec4(m_rules->m_emitDirection.x, m_rules->m_emitDirection.y, m_rules->m_emitDirection.z, 1.0f);
			glm::vec3 randomDir = transform * tuple;

			// 随机速度
			float speed = Random::getRandomNumber(m_rules->m_minSpeed, m_rules->m_maxSpeed);
			particle->velocity = randomDir * speed;

			// 加速度
			particle->acceleration = m_rules->m_gravity;

			// 随机声明周期
			particle->lifetime = Random::getRandomNumber(m_rules->m_minLifetime, m_rules->m_maxLifetime);
			particle->remainingTime = particle->lifetime;

			// 初始大小和颜色
			particle->size = m_rules->startSize;
			particle->color = m_rules->startColor;

	}


	void ParticleEmitter::udpate(float deltaTime) {
		const auto& rules = m_controller.getRules();
		XE_CORE_ASSERT((rules->m_emitRate <= 0.0f) ? false : true, "Don't have any particle!");

		float emitInterval = 1.0f / rules->m_emitRate; // 发射间隔
		m_emitTimer += deltaTime;
		while (m_emitTimer >= emitInterval) {
			emitParticle();
			m_emitTimer -= emitInterval;
		}
	}

	void ParticleEmitter::emitParticle() {
		Particle* particle = m_pool.getFreeParticle();
		if (!particle) return;
		m_controller.generateRandomParicleProps(particle);
	}

	void ParticleEmitter::burstEmit(unsigned int count) {
		for (unsigned int i = 0; i < count; ++i) {
			emitParticle();
		}
	}

	void ParticleUpdater::update(float deltaTime) {
		auto activeParticles = m_pool.getAcitiveParticles();
		const auto& rules = m_controller.getRules();

		for (auto* particle : activeParticles) {
			// 1. 更新生命周期
			particle->remainingTime -= deltaTime;
			if (particle->remainingTime <= 0) {
				m_pool.releaseParticle(particle);
				continue;
			}

			// 2. 更新物理状态(V = Vo + at; X = Xo + Vt)
			particle->velocity += particle->acceleration * deltaTime;
			// 阻尼 V = V*(1 - C)*t
			particle->velocity *= (1.0f - rules->m_damping * deltaTime);
			particle->position += particle->velocity * deltaTime;

			// 3. 视觉属性插值 (大小、颜色随生命周期渐变)
			float lifeRatio = 1.0f - (particle->remainingTime / particle->lifetime);
			// 大小插值
			particle->size = Math::Lerp(rules->startSize, rules->endSize, lifeRatio);
			// 颜色插值
			particle->color = Math::Lerp(rules->startColor, rules->endColor, lifeRatio);
		}
	}

	void PointParticleRenderer::render(const std::vector<Particle*>& activeParticles) {
		// 批量渲染 
		for (auto* particle : activeParticles) {
			Renderer2D::DrawQuad(particle->position, { particle->size, particle->size }, particle->color);
		}
		
	}

	void SpriteParticleRenderer::render(const std::vector<Particle*>& activeParticles) {
		for (auto* particle : activeParticles) {
			Renderer2D::DrawQuad(particle->position, { particle->size, particle->size }, particle->rotation, m_spriteTexture);
		}
	}

}