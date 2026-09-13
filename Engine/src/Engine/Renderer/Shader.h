#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& _path);
		static Ref<Shader> Create(const std::string& _name, const std::string& _vertex, const std::string& _fragment);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& _s);
		void Add(const std::string& _name, const Ref<Shader>& _s);
		Ref<Shader> Load(const std::string& _filepath);
		Ref<Shader> Load(const std::string& _name, const std::string& _filepath);
		Ref<Shader> Load(const std::string& _name, const std::string& _vertex, const std::string& _fragment);
	
		Ref<Shader> Get(const std::string& _name);
		bool Exists(const std::string& _name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
