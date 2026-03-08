#pragma once

#include "XEngine/Core/core.h"
#include "XEngine/Scene/Scene.h"
#include "XEngine/Scene/Entity.h"

namespace XEngine {
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void  SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::vector<Entity> m_EntitiesToDelete;
		std::vector<Entity> m_ComponentToDelete;
		
	};
}
