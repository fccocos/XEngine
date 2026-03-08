#pragma once
#include <entt.hpp>
#include "Scene.h"
namespace XEngine {
	class Entity
	{
	public:
		
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(const entt::entity& en, Scene* sc) :m_EntityHandle{ en }, m_Scene{ sc } {}

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&& ...args) {
			XE_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this,component);
			return component;
		}

		template<typename T>
		T& GetComponent() {
			XE_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		T* Try_GetComponent() {
			if (auto* component = m_Scene->m_Registry.try_get<T>(m_EntityHandle)) {
				return component;
			}
			else {
				std::cout << "component is not exists" << std::endl;
				return nullptr;
			}
		}

		template<typename T>
		void RemoveComponent() {
			XE_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null;}
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const {
			//return !operator==(other);
			return !(*this == other);
		}
		operator entt::entity() const { return m_EntityHandle; }
	private:
		entt::entity m_EntityHandle{entt::null};
		Scene* m_Scene = nullptr;
	};
}


