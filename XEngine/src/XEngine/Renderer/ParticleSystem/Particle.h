#pragma once
#include <glm/glm.hpp>
#include "XEngine/Renderer/Renderer2D.h"
#include "XEngine/Renderer/Texture.h"

namespace XEngine {
	/// <summary>
	/// 单个粒子的数据载体，存储 位置、速度、生命周期、颜色、大小等核心属性
	/// </summary>
	struct Particle
	{
		// 基础属性
		glm::vec3 position = {0.0f, 0.0f, 0.0f};         // 世界坐标
		glm::vec3 velocity = {0.0f, 0.0f, 0.0f};         // 速度向量
		glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};     // 加速度 (F = ma)
		float lifetime = 0.0f;                           // 总生命周期(s)
		float remainingTime = 0.0f;                      // 剩余生命周期(s)
		float size = 0.0f;                               // 粒子大小
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f};     // 粒子颜色
		float rotation = 0.0f;                           // 旋转角度

		// 状态标志
		bool isActive = false;                           // 是否激活(参与更新/渲染)

		// 粒子唯一标识符号
		unsigned int ID;

		// 重置粒子状态(对象池复用)
		void reset();

		bool operator==(const Particle& other) {
			return this->ID == other.ID ? true : false;
		}
	};

	/// <summary>
	/// 单例模式:
	/// 粒子对象池，复用粒子实例，避免内存碎片，管理粒子的激活 / 休眠状态
	/// </summary>
	class ParticlePool {
	public:
		// 删除拷贝构造和拷贝赋值运算符
		ParticlePool(const ParticlePool&) = delete;
		ParticlePool& operator=(const ParticlePool&) = delete;

		/// <summary>
		/// 获取空闲粒子（激活状态）
		/// </summary>
		/// <returns>返回一个空闲的粒子指针(无空闲返回nullptr)</returns>
		Particle* getFreeParticle();

		/// <summary>
		/// 释放粒子所占内存（睡眠）
		/// </summary>
		void releaseParticle(Particle* particle);

		/// <summary>
		/// 返回表示活动粒子的指针列表（std::vector<Paritcle*>）。
		/// </summary>
		/// <returns>包含指向 Particle 对象的指针的 std::vector，每个元素对应一个活动粒子。</returns>
		std::vector<Particle*> getAcitiveParticles();

		/// <summary>
		/// 将对象的内容清空或将其状态重置为默认/空状态。(设置为休眠)
		/// </summary>
		void clear();

		/// <summary>
		/// 静态函数，返回对 ParticlePool 单例实例的引用；使用 maxcount 指定池的最大粒子数（用于创建或调整实例）。
		/// </summary>
		/// <param name="maxcount">指定粒子池的最大容量（允许的最大粒子数）。</param>
		/// <returns>指向 ParticlePool 实例的引用（通常为单例），表示已按指定最大数量初始化或调整的粒子池。</returns>
		static ParticlePool& getParticlePoolInstance(unsigned int maxcount);
	private:
		ParticlePool(unsigned int maxcount);
		~ParticlePool() = default;
	private:
		std::vector<Particle> m_particles;
		unsigned int m_maxCount;
	};


	/// <summary>
	/// 粒子行为规则配置
	/// </summary>
	struct ParticleRules {
		/* 常量定义 */
		static const unsigned int MAXPARTICLES = 100000;

		/* 发射规则 */
		float        m_emitRate;          // 每秒发射粒子数
		unsigned int m_maxParticles;      // 最大同时粒子数量
		glm::vec3    m_emitPosition;      // 发射位置
		float        m_emitRange;         // 发射位置随机范围
		glm::vec3    m_emitDirection;     // 基础发射方向
		float        m_directionVariance; // 方向随机偏差(度)
		float        m_minSpeed;          // 最小初始速度
		float        m_maxSpeed;          // 最大初始速度

		/* 物理规则 */
		glm::vec3    m_gravity;           // 重力加速度
		float        m_damping;           // 速度阻尼(空气阻力,0~1) F = -CV (C为阻尼系数， V为速度)

		/* 生命周期规则 */
		float        m_minLifetime;       // 最小生命周期
		float        m_maxLifetime;       // 最大生命周期

		// 视觉规则
		float        startSize;           // 初始大小
		float        endSize;             // 结束大小
		glm::vec4    startColor;          // 初始颜色
		glm::vec4    endColor;            // 结束颜色

	};

	/// <summary>
	/// 单例：规则控制器，封装粒子的行为规则（如重力参数、颜色渐变、速度衰减）
	/// </summary>
	class ParticleController {
	public:
		// 初始化规则
		ParticleController(const Ref<ParticleRules>& rules) : m_rules(rules) {}
		ParticleController() = default;
		~ParticleController() = default;

		// 更新规则
		void updateRules(const Ref<ParticleRules>& newRules) { m_rules = newRules; }

		// 获取当前规则
		const Ref<ParticleRules>& getRules() const { return m_rules; }

		// 生成粒子的随机初始属性(基于规则)
		void generateRandomParicleProps(Particle* paricle);
		
	private:
		Ref<ParticleRules> m_rules;
	};


	/// <summary>
	/// 负责粒子的生成（发射），控制发射速率、方向、数量、初始属性
	/// </summary>
	class ParticleEmitter {
	public:
		ParticleEmitter(ParticlePool& pool, ParticleController& cotroller) :
			m_pool{ pool }, m_controller{ cotroller }, m_emitTimer{0.0f} {
		}

		~ParticleEmitter() {};

		/// <summary>
		/// 帧更新： 更具发生速率生成粒子
		/// </summary>
		/// <param name="daltaTime"></param>
		void udpate(float daltaTime);

		/// <summary>
		/// 发射单个粒子
		/// </summary>
		void emitParticle();

		/// <summary>
		/// 立即发射指定数量的粒子
		/// </summary>
		void burstEmit(unsigned int count);


	private:
		ParticlePool& m_pool;              // 粒子对象池（引用）
		ParticleController& m_controller;  // 规则控制器(引用)
		float m_emitTimer;                   // 发射计时器 (控制发色速率)
	};

	

	/// <summary>
	/// 负责粒子的帧更新，包括物理运动（重力、加速度）、生命周期、属性插值
	/// </summary>
	class ParticleUpdater {
	public:
		ParticleUpdater(ParticlePool& pool, ParticleController& controller) :
			m_pool(pool), m_controller(controller) {
		}

		void update(float deltaTime);


	private:
		ParticlePool& m_pool;
		ParticleController& m_controller;
	};

	/// <summary>
	/// 负责粒子的渲染，支持不同渲染方式（点渲染、精灵渲染、模型渲染）
	/// </summary>
	class ParticleRendererInterface {
	public:
		virtual ~ParticleRendererInterface() = default;
		// 渲染所有激活的例子
		virtual void render(const std::vector<Particle*>& activeParticles) = 0;
	};
	
	class PointParticleRenderer :public ParticleRendererInterface {
	public:
		void render(const std::vector<Particle*>& activeParticles) override;
	};

	
	/// <summary>
	///  精灵渲染实现(带纹理的例子: 如火焰、烟雾)
	/// </summary>
	class SpriteParticleRenderer : public ParticleRendererInterface {
	public:
		SpriteParticleRenderer(const Ref<Texture2D>& texture) :m_spriteTexture{ texture } {}

		void render(const std::vector<Particle*>& activeParticles) override;
	private:
		Ref<Texture2D> m_spriteTexture;
	};


	
	/// <summary> 
	/// 单例: 全局管理器，协调所有模块，提供启动 / 暂停 / 停止粒子系统的入口
	/// </summary>
	class ParticleSystemManager {
	public:
		ParticleSystemManager(int maxParticles, const Ref<ParticleRules>& rules, std::unique_ptr<ParticleRendererInterface> renderer) :
			m_pool{ ParticlePool::getParticlePoolInstance(maxParticles) },
			m_controller{ rules },
			m_emitter{ m_pool, m_controller },
			m_updater{ m_pool, m_controller },
			m_renderer{ std::move(renderer) },
			m_isRunning{ false } {
		}

		// 启动粒子
		void start() { m_isRunning = true; }
		// 暂停粒子系统
		void stop() { m_isRunning = false; m_pool.clear(); }
		// 帧更新
		void update(float deltaTime) {
			XE_CORE_ASSERT(m_isRunning, "Particle system is not running!");

			// 发射粒子
			m_emitter.udpate(deltaTime);
			// 更新器更新粒子状态
			m_updater.update(deltaTime);
			// 渲染器渲染粒子
			m_renderer->render(m_pool.getAcitiveParticles());
		}

		// 立即发射一批粒子(爆炸、特效)
		void burst(int count) {
			if (m_isRunning) {
				m_emitter.burstEmit(count);
			}
		}

		// 更新规则
		void udpateRules(const Ref<ParticleRules>& newRules) {
			m_controller.updateRules(newRules);
		}

		// 切换渲染器
		void setRenderer(std::unique_ptr<ParticleRendererInterface> newRenderer) {
			m_renderer = std::move(newRenderer);
		}

		void clearPool() { m_pool.clear(); }
	private:
		ParticlePool& m_pool;
		ParticleController m_controller;
		ParticleEmitter m_emitter;
		ParticleUpdater m_updater;
		std::unique_ptr<ParticleRendererInterface> m_renderer;
		bool m_isRunning;
	};

}

