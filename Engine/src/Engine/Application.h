#pragma once

#include "Core.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		void Run();
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

