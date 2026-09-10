#pragma once

#include <Engine/Window.h>

#include <GLFW/glfw3.h>

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& _props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.width; }
		inline unsigned int GetHeight() const override { return m_Data.height; }

		inline void SetEventCallback(const EventCallbackFn& _callback) override { m_Data.eventCallback = _callback; }
		void SetVSync(bool _enable) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; };

	private:
		virtual void Init(const WindowProps& _props);
		virtual void Shutdown();

		GLFWwindow* m_Window = nullptr;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;

			EventCallbackFn eventCallback;
		};

		WindowData m_Data;
	};
}