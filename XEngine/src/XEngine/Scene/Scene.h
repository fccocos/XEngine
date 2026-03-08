#pragma once
#include <entt.hpp>
#include "XEngine/Core/TimeStep.h" 
#include <memory>
namespace XEngine {

	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
	public:
		Scene() = default;
		~Scene() =default;
		void DestroyEntity(Entity entity);

		Entity CreateEntity(const std::string& name="");
		void OnUpdate(TimeStep ts);

		void OnViewportResize(unsigned int width, unsigned int height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry; // 实体上文
		unsigned int m_ViewportWidth, m_ViewportHeight;
		
	};

}

