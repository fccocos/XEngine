#pragma once

#include "XEngine/Renderer/OrthographicCamera.h"
#include "XEngine/Core/TimeStep.h"
#include "XEngine/Event/Event.h"
#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/Event/MouseEvent.h"

namespace XEngine {
	class OrthographicCameraController {
	public:
		explicit OrthographicCameraController(float aspectRatio, bool rotation=false); // aspectratio * 2 units

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio; // 宽高比
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition{ 0.0f };
		float m_CameraRotation{ 0.0f };

		float m_CameraTranslateSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;

	};
}
