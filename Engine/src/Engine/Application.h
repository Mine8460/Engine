#pragma once

#include "Core.h"
#include "Window.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_running = true;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

