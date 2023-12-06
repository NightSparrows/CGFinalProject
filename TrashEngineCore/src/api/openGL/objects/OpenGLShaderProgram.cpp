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
		GLint location = glGetUniformLocation(this->m_programHandle, uniformName);

		if (location != -1)
			this->m_locations[uniformName] = location;
		else
			NS_CORE_ERROR("location name: {} not found", uniformName);

		return location;
	}

	void OpenGLShaderProgram::loadFloat(const std::string& uniformName, float value)
	{
		this->load(this->getUniformLocation(uniformName), value);
	}

	void OpenGLShaderProgram::loadUInt(const std::string& uniformName, uint32_t value)
	{
		glProgramUniform1ui(this->m_programHandle, this->getUniformLocation(uniformName), value);
	}

	void OpenGLShaderProgram::loadInt(const std::string& uniformName, GLint value)
	{
		glProgramUniform1i(this->m_programHandle, this->getUniformLocation(uniformName), value);
	}

	void OpenGLShaderProgram::loadIVec2(const std::string& uniformName, const glm::ivec2& value)
	{
		glProgramUniform2iv(this->m_programHandle, this->getUniformLocation(uniformName), 1, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::loadVec3(const std::string& uniformName, const glm::vec3& value)
	{
		glProgramUniform3fv(this->m_programHandle, this->getUniformLocation(uniformName), 1, glm::value_ptr(value));
	}

	GLint OpenGLShaderProgram::getUniformLocation(const std::string& uniformName)
	{
		return this->m_locations[uniformName];
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
