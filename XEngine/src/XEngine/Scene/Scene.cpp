#include "xepch.h"
#include "glm/glm.hpp"

#include "Components.h"
#include "XEngine/Renderer/Renderer2D.h"
#include "Entity.h"
#include "ScriptableEnttity.h"

namespace XEngine {

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = Entity(m_Registry.create(), this );

		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		if (entity) 
			if(m_Registry.valid(entity))
				m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(TimeStep ts) {

		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScriptFn();
					nsc.Instance->m_Entity = Entity(entity, this);
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
		
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto [entity, trans, cam] : view.each()) {
				if (cam.Primary) {
					mainCamera = &cam.CameraInsatnce;
					cameraTransform = &trans.GetTransform();
					break;
				}
			}
			
		}

		if (mainCamera) {
			Renderer2D::BeginScense(mainCamera->GetProjection(), *cameraTransform);
			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent, PaletteComponent>();
			for (auto entity : group) {
				auto [trans, sprite, colorPalette] = group.get<TransformComponent, SpriteRendererComponent, PaletteComponent>(entity);
				Renderer2D::DrawQuad(trans.GetTransform(), sprite );
				//Renderer2D::DrawQuad(trans.GetTransform(), colorPalette.Color);
			}

			Renderer2D::EndScense();

		}
		
		
	}

	void Scene::OnViewportResize(unsigned int width, unsigned int height) {
		m_ViewportHeight = height;
		m_ViewportWidth = width;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.CameraInsatnce.SetViewportSize(width, height);
			}
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	// 变换
	template<> void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {

	}

	// 相机
	template<> void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.CameraInsatnce.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	// 原生脚本
	template<> void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {

	}

	// 调色盘
	template<> void Scene::OnComponentAdded<PaletteComponent>(Entity entity, PaletteComponent& component) {

	}

	// 精灵渲染
	template<> void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {

	}

	// 网格
	template<> void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {

	}

	// 标签
	template<> void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {

	}
}

