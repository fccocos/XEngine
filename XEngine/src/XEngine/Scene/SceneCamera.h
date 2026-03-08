#pragma once

#include "XEngine/Renderer/Camera.h"

namespace XEngine {
	class SceneCamera: public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic =1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(unsigned int width, unsigned int height);

		// Perspective
		void  SetPerspective(float verticalFOV, float nearClip, float farClip);
		float GetPerspectiveVerticalFOV() const            { return m_PerspectiveFOV;                               }
		void  SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveFOV = verticalFOV; RecaluateProjection(); }
		float GetPerspectiveNearClip() const               { return m_PerspectiveNear;                              }
		float GetPerspectiveFarClip() const                { return m_PerspectiveFar;                               }
		void  SetPerspectiveNearClip(float nearSize)       { m_PerspectiveNear = nearSize;   RecaluateProjection(); }
		void  SetPerspectiveFarClip(float farSize)         { m_PerspectiveFar  = farSize;    RecaluateProjection(); }

		// Orthographic
		float GetOrthographicSize() const                  { return m_OrthographicSize;                             }
		void  SetOrthographicSize(float size)              { m_OrthographicSize = size;      RecaluateProjection(); }
		float GetOrthographicNearClip() const              { return m_OrthographicNear;                             }
		float GetOrthographicFarClip() const               { return m_OrthographicFar;                              }
		void  SetOrthographicNearClip(float nearSize)      { m_OrthographicNear = nearSize;  RecaluateProjection(); }
		void  SetOrthographicFarClip(float farSize)        { m_OrthographicFar = farSize;    RecaluateProjection(); }

		ProjectionType GetPorjectionType()const            { return m_ProjectionType;                               }
		void setProjectionType(ProjectionType type)        { m_ProjectionType = type;        RecaluateProjection(); }

	private:
		void RecaluateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 1.0f;

	};
}


