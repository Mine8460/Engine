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
	
		static Shader* Create(const std::string& _vertex, const std::string& _fragment);
	};
}
