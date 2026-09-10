#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "ImGui/ImGuiLayer.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		void Run();

		void OnEvent(Event& _e);

		void PushLayer(Layer* _layer);
		void PushOverlay(Layer* _overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance;  }
	private:
		bool OnWindowClosed(WindowCloseEvent& _e);

		static Application* s_Instance;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

