#include "Enpch.h"
#include "Shader.h"

#include <Engine/Renderer/RendererAPI.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace Engine
{
	Ref<Shader> Shader::Create(const std::string& _path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(_path);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::Direct3D: return nullptr;
		}
		return nullptr;

		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& _name, const std::string& _vertex, const std::string& _fragment)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(_name, _vertex, _fragment);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::Direct3D: return nullptr;
		}
		return nullptr;
	}
	void ShaderLibrary::Add(const Ref<Shader>& _s)
	{
		m_Shaders[_s->GetName()] = _s;
	}
	void ShaderLibrary::Add(const std::string& _name, const Ref<Shader>& _s)
	{
		m_Shaders[_name] = _s;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& _filepath)
	{
		auto shader = Shader::Create(_filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& _name, const std::string& _filepath)
	{
		auto shader = Shader::Create(_filepath);
		Add(_name, shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& _name, const std::string& _vertex, const std::string& _fragment)
	{
		auto shader = Shader::Create(_name, _vertex, _fragment);
		Add(_name, shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& _name)
	{
		return m_Shaders[_name];
	}
	bool ShaderLibrary::Exists(const std::string& _name)
	{
		return m_Shaders.find(_name) != m_Shaders.end();
	}
}
