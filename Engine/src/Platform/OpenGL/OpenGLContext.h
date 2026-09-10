#pragma once

#include <Engine/Renderer/GraphicsContext.h>
#include <GLFW/glfw3.h>

namespace Engine
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* _winHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle = nullptr;
	};

}

