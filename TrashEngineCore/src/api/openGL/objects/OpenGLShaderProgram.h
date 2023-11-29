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

	protected:

		GLuint m_programHandle;

	};

}
