#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "XEngine/Scene/SceneCamera.h"
#include "XEngine/Scene/ScriptableEnttity.h"
namespace XEngine {

	struct TagComponent {
		std::string Tag{};
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag{ tag } {}
		operator const std::string& () { return Tag; }
		operator std::string() { return Tag; }
	};

	struct TransformComponent {

		glm::vec3 Translate = glm::vec3(0.0f);
		glm::vec3 Rotate = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& trans) : Translate{ trans } {}
		
		glm::mat4  GetTransform() const {
			const glm::vec3 rotateX = { 1, 0, 0 };
			const glm::vec3 rotateY = { 0, 1, 0 };
			const glm::vec3 rotateZ = { 0, 0, 1 };
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotate.x, rotateX) *
								glm::rotate(glm::mat4(1.0f), Rotate.y, rotateY) *
								glm::rotate(glm::mat4(1.0f), Rotate.z, rotateZ);

			return glm::translate(glm::mat4(1.0f), Translate) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	// 调色板组件
	struct PaletteComponent {
		glm::vec4 Color = glm::vec4(1.0f);

		PaletteComponent() = default;
		PaletteComponent(const PaletteComponent&) = default;
		PaletteComponent(const glm::vec4& color) : Color{ color } {}
		operator const glm::vec4& () { return Color; }
		operator glm::vec4() { return Color; }
	};

	// 纹理加载器
	struct TextureLoaderComponent {
		
	};

	// 粒子系统
	struct ParticleSystemComponent {

	};

	struct MeshComponent {

	};

	struct SpriteRendererComponent {
		glm::vec4 Color=glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color{ color } {}
		operator const glm::vec4& () { return Color; }
		operator glm::vec4() { return Color; }

	};

	struct CameraComponent {

		SceneCamera CameraInsatnce;

		bool Primary = true;
		bool FixedAspectRatio = true;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		/*CameraComponent(const glm::mat4& projection){}*/
		/*operator const glm::mat4& () { return RendererCamera.GetProjection(); }
		operator glm::mat4() { return RendererCamera.GetProjection(); }*/
	};

	struct NativeScriptComponent {

		using InstantiateScript = ScriptableEntity * (*)();
		using DestroyInstanceScript = void (*)(NativeScriptComponent*);
		
		ScriptableEntity* Instance = nullptr;

		InstantiateScript InstantiateScriptFn; // 创建脚本实例
		DestroyInstanceScript DestroyInstanceScriptFn;// 销毁脚本实例

		//std::function<void(ScriptableEntity*)> OnCreateFunction;  // 创建
		//std::function<void(ScriptableEntity*)> OnDestoryFunction; // 销毁
		//std::function<void(ScriptableEntity*,TimeStep)> OnUpdateFunction;  // 更新

		template<typename T>
		void Bind() {
			InstantiateScriptFn = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceScriptFn = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };

			/*OnCreateFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnCreate(); };
			OnDestoryFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, TimeStep ts) { static_cast<T*>(instance)->OnUpdate(ts); };*/
		}
	};
}
