#include <Enpch.h>
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

namespace Engine
{

	Application::Application(void)
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application(void)
	{
	}

	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}