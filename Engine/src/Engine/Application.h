#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

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
	private:
		bool OnWindowClosed(WindowCloseEvent& _e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

