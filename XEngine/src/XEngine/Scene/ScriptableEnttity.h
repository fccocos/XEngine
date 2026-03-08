#pragma once

#include "Entity.h"

namespace XEngine {
	class ScriptableEntity {
		friend class Scene;
	public:


		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnDestroy() {}
	private:
		Entity m_Entity;
		
	};
}
