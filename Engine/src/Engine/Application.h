#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		void Run();

		void OnEvent(Event& _e);
	private:
		bool OnWindowClosed(WindowCloseEvent& _e);

		std::unique_ptr<Window> m_Window;
		bool m_running = true;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

