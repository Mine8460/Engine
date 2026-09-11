#include "Enpch.h"
#include "Shader.h"

#include <Engine/Renderer/RendererAPI.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace Engine
{

	Shader* Shader::Create(const std::string& _vertex, const std::string& _fragment)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(_vertex, _fragment);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::Direct3D: return nullptr;
		}
		return nullptr;
	}
}
