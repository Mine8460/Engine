#pragma once

#include <glm/glm.hpp>

namespace Engine
{

	class Camera
	{
	public:
		Camera(void) = default;

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& _position) { m_Position = _position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::vec3& _rot) { m_Rotation = _rot; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		virtual void RecalculateViewMatrix(void) = 0;
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
	};

	class CameraController
	{
	public:
		CameraController(void) = default;
	private:
		Ref<Camera> m_Camera;
	};
}
#pragma once
