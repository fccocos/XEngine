#include "xepch.h"
#include "OrthographicCameraController.h"
#include "XEngine/Core/Input.h"
#include "XEngine/Core/KeyCodes.h"
#include "XEngine/Core/core.h"

namespace XEngine {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio*m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation), 
		m_Bounds{0.0f} {

	}

	void OrthographicCameraController::OnUpdate(TimeStep ts) {

		if (Input::IsKeyPressed(XE_KEY_A))
			m_CameraPosition.x += m_CameraTranslateSpeed * ts;
		else if (Input::IsKeyPressed(XE_KEY_D))
			m_CameraPosition.x -= m_CameraTranslateSpeed * ts;

		if (Input::IsKeyPressed(XE_KEY_W))
			m_CameraPosition.y -= m_CameraTranslateSpeed * ts;
		else if (Input::IsKeyPressed(XE_KEY_S))
			m_CameraPosition.y += m_CameraTranslateSpeed * ts;

		if (m_Rotation) {
			if (Input::IsKeyPressed(XE_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(XE_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}
		ReCalculateView();
		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispather dispather(e);
		
		dispather.Dispatch<MouseScrolledEvent>(EX_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispather.Dispatch<WindowResizeEvent>(EX_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
		
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset();
		ReCalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e) {
		OnResizeOrthographicCameraBounds(static_cast<float>(e.GetWidth()),static_cast<float>(e.GetHeigth()));
		return false;
	}

	void OrthographicCameraController::ReCalculateView() {
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	void OrthographicCameraController::OnResizeOrthographicCameraBounds(float width, float height) {
		m_AspectRatio = width / height;
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	void OrthographicCameraController::OnResizeOrthographicCameraBounds(const glm::vec2& size) {
		OnResizeOrthographicCameraBounds(size.x, size.y);
	}
}