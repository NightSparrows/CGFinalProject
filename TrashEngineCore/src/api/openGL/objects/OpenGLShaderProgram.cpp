#include "nspch.h"
#include <core/Logger.h>

#include "OpenGLShaderProgram.h"
#include <TrashEngine/deps/glm/gtc/type_ptr.hpp>

namespace TrashEngine {

	OpenGLShaderProgram::OpenGLShaderProgram()
	{
		this->m_programHandle = glCreateProgram();
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		glDeleteProgram(this->m_programHandle);
	}

	void OpenGLShaderProgram::bind()
	{
		glUseProgram(this->m_programHandle);
	}

	void OpenGLShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	GLint OpenGLShaderProgram::getUniformLocationFromName(const char* uniformName)
	{
		return glGetUniformLocation(this->m_programHandle, uniformName);
	}

	void OpenGLShaderProgram::load(GLint location, const glm::uvec2& value)
	{
		glProgramUniform2uiv(this->m_programHandle, location, 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, const glm::uvec4& value)
	{
		glProgramUniform4uiv(this->m_programHandle, location, 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, const glm::ivec2& value)
	{
		glProgramUniform2iv(this->m_programHandle, location, 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, const glm::ivec4& value)
	{
		glProgramUniform4iv(this->m_programHandle, location, 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, const glm::vec2& value)
	{
		glProgramUniform2fv(this->m_programHandle, location, 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, const glm::vec3& value)
	{
		glProgramUniform3fv(this->m_programHandle, location, 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, const glm::mat4& value)
	{
		glProgramUniformMatrix4fv(this->m_programHandle, location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::load(GLint location, float value)
	{
		glProgramUniform1f(this->m_programHandle, location, value);
	}

	void OpenGLShaderProgram::load(GLint location, GLint value)
	{
		glProgramUniform1i(this->m_programHandle, location, value);
	}

}
