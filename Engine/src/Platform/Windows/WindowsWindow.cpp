#include <Enpch.h>
#include "WindowsWindow.h"


namespace Engine
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& _props)
	{
		return new WindowsWindow(_props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& _props)
	{
		Init(_props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool _enable)
	{
		if (_enable)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = _enable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& _props)
	{
		m_Data.title = _props.title;
		m_Data.width = _props.width;
		m_Data.height = _props.height;

		if (!s_GLFWInitialized)
		{
			int succes = glfwInit();

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)_props.width, (int)_props.height, m_Data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}