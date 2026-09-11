#include "Enpch.h"
#include "Texture.h"

#include <Engine/Renderer/RendererAPI.h>
#include <Platform/OpenGL/OpenGLTexture.h>

namespace Engine
{
	Texture2D* Texture2D::Create(const std::string& _path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(_path);
		}
		return nullptr;
	}
}
