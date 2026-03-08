#pragma once

#include "Particle.h"

namespace XEngine {
	struct ParticlePresets {
		static ParticleRules fire() {
			ParticleRules rules{};
									  
			/* 发射规则 */ 
			rules.m_emitRate = 50.0f;
			rules.m_maxParticles = 200;
			rules.m_emitPosition = { 0.0f, 0.0f, 0.0f };
			rules.m_emitRange = 0.5f;
			rules.m_emitDirection = { 0.0f, 1.0f, 0.0f };
			rules.m_directionVariance = 20.0f;
			rules.m_minSpeed = 1.0f;
			rules.m_maxSpeed = 3.0f;
				                                     
			/* 物理规则 */                     
			rules.m_gravity = { 0.0f, -0.5f, 0.0f };
			rules.m_damping = 0.05f;
				                                  
			/* 生命周期规则 */                       
			rules.m_minLifetime = 0.8f;
			rules.m_maxLifetime = 1.5f;
				                                        
			// 视觉规则                            
			rules.startSize = 0.8f;
			rules.endSize = 0.2f;
			rules.startColor = { 1.0f, 0.5f, 0.0f, 1.0f };
			rules.endColor = { 1.0f, 0.0f, 0.0f, 0.0f };
			
			return rules;
		}

		static ParticleRules rain() {
			ParticleRules rules{};

			/* 发射规则 */
			rules.m_emitRate = 200.0f;
			rules.m_maxParticles = 1000;
			rules.m_emitPosition = { 0.0f, 10.0f, 0.0f };
			rules.m_emitRange = 0.5f;
			rules.m_emitDirection = { 0.0f, -1.0f, 0.0f };
			rules.m_directionVariance = 5.0f;
			rules.m_minSpeed = 8.0f;
			rules.m_maxSpeed = 12.0f;

			/* 物理规则 */
			rules.m_gravity = { 0.0f, -9.8f, 0.0f };
			rules.m_damping = 0.01f;

			/* 生命周期规则 */
			rules.m_minLifetime = 1.0f;
			rules.m_maxLifetime = 1.2f;

			// 视觉规则                            
			rules.startSize = 0.1f;
			rules.endSize = 0.1f;
			rules.startColor = { 0.8f, 0.9f, 1.0f, 0.8f };
			rules.endColor = { 0.8f, 0.9f, 1.0f, 0.8f };

			return rules;
		}

		static ParticleRules pureColor() {
			ParticleRules rules{};

			/* 发射规则 */
			rules.m_emitRate = 200.0f;
			rules.m_maxParticles = 1000;
			rules.m_emitPosition = { 0.0f, 10.0f, 0.0f };
			rules.m_emitRange = 0.5f;
			rules.m_emitDirection = { 0.0f, -1.0f, 0.0f };
			rules.m_directionVariance = 5.0f;
			rules.m_minSpeed = 2.0f;
			rules.m_maxSpeed = 4.0f;

			/* 物理规则 */
			rules.m_gravity = { 0.0f, 0.0f, 0.0f };
			rules.m_damping = 0.00f;

			/* 生命周期规则 */
			rules.m_minLifetime = 1.0f;
			rules.m_maxLifetime = 1.2f;

			// 视觉规则                            
			rules.startSize = 0.1f;
			rules.endSize = 0.1f;
			rules.startColor = { 0.8f, 0.3f, 0.2f, 0.8f };
			rules.endColor = { 0.1f, 0.3f, 0.2f, 0.8f };

			return rules;
		}
	};
}

