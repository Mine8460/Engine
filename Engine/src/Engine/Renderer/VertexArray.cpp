#include "Enpch.h"
#include "VertexArray.h"

#include <Engine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace Engine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		case RendererAPI::API::Direct3D: return nullptr;
		case RendererAPI::API::Vulkan: return nullptr;
		default:
			break;
		}

		return nullptr;
	}
}
