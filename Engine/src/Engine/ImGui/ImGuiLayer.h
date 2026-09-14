#pragma once

#include <Engine/Core/Layer.h>
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

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:

		ImGuiKey ConvertGLFWInputToImGui(int _key);
	private:
		float m_Time = 0.f;
	};
}
