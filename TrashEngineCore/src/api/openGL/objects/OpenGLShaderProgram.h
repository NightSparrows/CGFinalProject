#pragma once

#include <string>

#include <TrashEngine/deps/glm/glm.hpp>
#include <deps/glad/glad.h>

namespace TrashEngine {

	class OpenGLShaderProgram
	{
		friend class OpenGLShaderProgramBuilder;
	public:
		~OpenGLShaderProgram();

		void bind();
		void unbind();

		GLint getUniformLocationFromName(const char* uniformName);

		void loadFloat(const std::string& uniformName, float value);

		void loadUInt(const std::string& uniformName, uint32_t value);

		void loadInt(const std::string& uniformName, GLint value);

		void loadIVec2(const std::string& uniformName, const glm::ivec2& value);

		void loadVec3(const std::string& uniformName, const glm::vec3& value);

		void load(GLint location, const glm::uvec2& value);

		void load(GLint location, const glm::uvec4& value);

		void load(GLint location, const glm::ivec2& value);

		void load(GLint location, const glm::ivec4& value);

		void load(GLint location, const glm::vec2& value);

		void load(GLint location, const glm::vec3& value);

		void load(GLint location, const glm::mat4& value);

		void load(GLint location, float value);

		void load(GLint location, GLint value);

		GLuint getHandle() { return this->m_programHandle; }

	protected:
		OpenGLShaderProgram();

		// get location from cache
		GLint getUniformLocation(const std::string& uniformName);

	protected:

		std::map<std::string, GLint> m_locations;

		GLuint m_programHandle;

	};

}
