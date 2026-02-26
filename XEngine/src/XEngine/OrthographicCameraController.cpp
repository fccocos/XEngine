#include "xepch.h"
#include "OrthographicCameraController.h"
#include "XEngine/Input.h"
#include "XEngine/KeyCodes.h"
#include "XEngine/core.h"

namespace XEngine {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio*m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation){

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

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispather dispather(e);
		
		dispather.Dispatch<MouseScrolledEvent>(EX_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispather.Dispatch<WindowResizeEvent>(EX_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
		
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e) {
		m_ZoomLevel = (float)e.GetWidth()/(float)e.GetHeigth();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}