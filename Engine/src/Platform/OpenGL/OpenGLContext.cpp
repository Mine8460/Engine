#include "Enpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* _winHandle) : m_WindowHandle(_winHandle)
	{

	}

	void Engine::OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		ENGINE_CORE_INFO("OpenGL Info :");
		ENGINE_CORE_INFO((const char*)glGetString(GL_VENDOR));
		ENGINE_CORE_INFO((const char*)glGetString(GL_RENDERER));
		ENGINE_CORE_INFO((const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}
