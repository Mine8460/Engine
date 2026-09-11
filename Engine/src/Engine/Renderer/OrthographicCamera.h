#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float _left, float _right, float _bottom, float _top);
	
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& _position) { m_Position = _position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float _rot) { m_Rotation = _rot; RecalculateViewMatrix(); }
	
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix(void);
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
	};
}
