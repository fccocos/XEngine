#pragma once

#include "XEngine/Renderer/OrthographicCamera.h"
#include "XEngine/Core/TimeStep.h"
#include "XEngine/Event/Event.h"
#include "XEngine/Event/ApplicationEvent.h"
#include "XEngine/Event/MouseEvent.h"

namespace XEngine {

	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() const { return Right - Left; }
		float GetHeight() const { return Top - Bottom; }
	};

	class OrthographicCameraController {
	public:
		explicit OrthographicCameraController(float aspectRatio, bool rotation=false); // aspectratio * 2 units

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) {
			m_ZoomLevel = zoomLevel;
			ReCalculateView();
		}

		const OrthographicCameraBounds& GetOrthographicCameraBounds() const { return m_Bounds; }
		

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
		void OnResizeOrthographicCameraBounds(float width, float height);
		void OnResizeOrthographicCameraBounds(const glm::vec2& size);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ReCalculateView();
	private:
		float m_AspectRatio; // 宽高比
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_Bounds;

		bool m_Rotation;
		glm::vec3 m_CameraPosition{ 0.0f };
		float m_CameraRotation{ 0.0f };

		float m_CameraTranslateSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;

	};
}
