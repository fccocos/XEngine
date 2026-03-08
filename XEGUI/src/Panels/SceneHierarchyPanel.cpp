#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <XEngine/Scene/Components.h>
#include <XEngine/Debug/Instrumentor.h>
#include <glm/gtc/type_ptr.hpp>

namespace XEngine {

	static void DrawVec4PaletteControl(glm::vec4& color, float resetValue = 0.0f, float paletteWidth =400.0f) {
		ImGui::PushID("Palette");
		float windowWidth = ImGui::GetContentRegionAvail().x;
		float offsetX = (windowWidth - paletteWidth) * 0.5f;
		ImGui::SetNextItemWidth(paletteWidth);
		ImGui::SetCursorPosX(offsetX);
		ImGui::Spacing();
		ImGui::ColorPicker3("##MyColor##6", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		ImGui::Spacing();
		ImGui::ColorEdit4("HSV shown as RGB##1", (float*)&color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
		ImGui::Spacing();
		ImGui::PopID();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 60.0f) {
		
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		float totalWidth = ImGui::CalcItemWidth();
		ImGui::PushMultiItemsWidths(3, totalWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0, 0 });

		float lineHight = ImGui::GetFontSize() + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHight + 3.0f, lineHight };

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.20f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.10f, 0.15f, 1.0f });
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
		}

		
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
		}
		

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string name, Entity entity, UIFunction uiFunction) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap;

		// 变换
		if (entity.HasComponent<T>()) {

			auto component = entity.GetComponent<T>();
			auto contentRegionAvailX = ImGui::GetContentRegionAvail().x;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = ImGui::GetFontSize() + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailX - lineHeight * 0.5);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removecomponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove component")) {
					removecomponent = true;
				}

				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
				
			}

			if (removecomponent)
			    entity.RemoveComponent<T>();

		}
	}
	
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}

	void  SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		XE_PROFILE_FUNCTION();

		ImGui::Begin("Scene Hierarchy"); {
			for (auto entity : m_EntitiesToDelete) {
				if (m_SelectionContext == entity) {
					m_SelectionContext = {};
				}
				m_Context->DestroyEntity(entity);
			}
			m_EntitiesToDelete.clear();
	
				// 使用集合存储已处理的实体ID
			std::unordered_set<entt::entity> processedEntities;

			for (auto [id, storage] : m_Context->m_Registry.storage()) {
				for (auto entityID : storage) {
					// 检查实体是否有效且未处理过
					if (m_Context->m_Registry.valid(entityID) && processedEntities.find(entityID) == processedEntities.end()) {
						// 检查实体是否有TagComponent
						if (m_Context->m_Registry.all_of<TagComponent>(entityID)) {
							Entity entity = { entityID, m_Context.get() };
							DrawEntityNode(entity);
							// 标记为已处理
							processedEntities.insert(entityID);
						}
					}
				}
			}

			if (ImGui::BeginPopupContextWindow("Props", ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::MenuItem("Create Entity")) {
					Entity entity = m_Context->CreateEntity("New Entity");
					m_SelectionContext = entity;
				}
				if (ImGui::MenuItem("Create Camera")) {
					Entity entity = m_Context->CreateEntity("Camera");
					m_Context->m_Registry.emplace<CameraComponent>(entity);
					m_SelectionContext = entity;
				}
				if (ImGui::MenuItem("Create Light")) {
					Entity entity = m_Context->CreateEntity("Light");
					m_Context->m_Registry.emplace<SpriteRendererComponent>(entity);
					m_SelectionContext = entity;
				}

				ImGui::EndPopup();
			}
			
		}ImGui::End();


		ImGui::Begin("Props");{
			if (m_SelectionContext) {
				DrawComponents(m_SelectionContext);
			}
			
		}ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
		XE_PROFILE_FUNCTION();
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity)?ImGuiTreeNodeFlags_Selected:0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)((uint64_t)(uint32_t)entity), flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}

		bool delayDeleteEntityID = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity")) {
				delayDeleteEntityID = true;
			}
			ImGui::EndPopup();
		}

		if (opened) {
			// 可以绘制子节点
			ImGui::TreePop();
		}

		if (delayDeleteEntityID) {
			m_EntitiesToDelete.push_back(entity);
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {

		// 标签


		if (entity.HasComponent<TagComponent>()) {
			
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256]{};
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) {
			if (ImGui::MenuItem("Camera")) {
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Palette")) {
				m_SelectionContext.AddComponent<PaletteComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		

		// 变换
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Translate", component.Translate);
			DrawVec3Control("Rotate", component.Rotate);
			DrawVec3Control("Scale", component.Scale);
		});

		
		
		// 相机组件
		DrawComponent<CameraComponent>("Camera", entity, [](auto& cameraComponent){
			auto& camera = cameraComponent.CameraInsatnce;

			ImGui::Checkbox("Primary", &cameraComponent.Primary);
			const char* projectionTypeStrings[] = { "Perspective","Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int>(camera.GetPorjectionType())];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {

				for (int i = 0; i < 2; i++) {
					bool isSeleted = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSeleted)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.setProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSeleted) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (camera.GetPorjectionType() == SceneCamera::ProjectionType::Perspective) {
				float perFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("FOV", &perFOV))
					camera.SetPerspectiveVerticalFOV(perFOV);

				float perNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perNear))
					camera.SetPerspectiveNearClip(perNear);

				float perFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perFar))
					camera.SetPerspectiveFarClip(perFar);
			}

			if (camera.GetPorjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

			}

		});
		/*if (entity.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera Props")) {
				auto& cameraComponet = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponet.CameraInsatnce;

				ImGui::Checkbox("Primary", &cameraComponet.Primary);


				const char* projectionTypeStrings[] = { "Perspective","Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetPorjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {

					for (int i = 0; i < 2; i++) {
						bool isSeleted = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSeleted)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.setProjectionType((SceneCamera::ProjectionType)i);

						}

						if (isSeleted) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (camera.GetPorjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("FOV", &perFOV))
						camera.SetPerspectiveVerticalFOV(perFOV);

					float perNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perNear))
						camera.SetPerspectiveNearClip(perNear);

					float perFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perFar))
						camera.SetPerspectiveFarClip(perFar);
				}

				if (camera.GetPorjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponet.FixedAspectRatio);

				}

				ImGui::TreePop();
			}

			ImGui::SeparatorText("");
		}*/

		// 调色盘
		DrawComponent<PaletteComponent>("Camera", entity, [](auto& component) {
			DrawVec4PaletteControl(component.Color);
		});
		/*if (entity.HasComponent<PaletteComponent>()) {

			if (ImGui::TreeNodeEx((void*)typeid(PaletteComponent).hash_code(), treeNodeFlags, "Color Palette")) {
				auto& color = entity.GetComponent<PaletteComponent>().Color;
				DrawVec4PaletteControl(color);
				ImGui::TreePop();
			}
		}*/

		// 精灵组件
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			DrawVec4PaletteControl(component.Color);
		});
		
	}
}