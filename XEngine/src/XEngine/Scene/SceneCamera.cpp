#include "xepch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>


namespace XEngine {

	SceneCamera::SceneCamera(){
		RecaluateProjection();
	}

	void  SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip) {
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecaluateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecaluateProjection();
	}

	void SceneCamera::SetViewportSize(unsigned int width, unsigned int height) {
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecaluateProjection();
	}

	void SceneCamera::RecaluateProjection() {
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);

		}else {
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}

		
	}

}
