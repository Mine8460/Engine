#pragma once

#include <Engine/Layer.h>
#include <Engine/Events/ApplicationEvent.h>
#include <Engine/Events/MouseEvent.h>
#include <Engine/Events/KeyEvent.h>

#include "../../../vendor/imgui/imgui.h"

namespace Engine
{
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& _event) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypeEvent(KeyTypedEvent& e);
		bool OnWindowResizeddEvent(WindowResizeEvent& e);

		ImGuiKey ConvertGLFWInputToImGui(int _key);
	private:
		float m_Time = 0.f;
	};
}
