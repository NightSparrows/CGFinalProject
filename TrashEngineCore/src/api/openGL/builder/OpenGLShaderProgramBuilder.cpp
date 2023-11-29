#include "nspch.h"
#include <core/Logger.h>
#include "OpenGLShaderProgramBuilder.h"

namespace TrashEngine {

    OpenGLShaderProgram* OpenGLShaderProgramBuilder::build()
    {
		OpenGLShaderProgram* program = new OpenGLShaderProgram();

		for (GLuint shader : this->m_shaders) {
			glAttachShader(program->m_programHandle, shader);
		}
		glLinkProgram(program->m_programHandle);

		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program->m_programHandle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program->m_programHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program->m_programHandle, maxLength, &maxLength, infoLog.data());
			delete program;

			NS_CORE_ERROR("Failed to link shader program.");
			NS_CORE_ERROR("{0}", infoLog.data());

			return nullptr;
		}
		for (GLuint shader : this->m_shaders) {
			glDetachShader(program->m_programHandle, shader);
		}
		glValidateProgram(program->m_programHandle);

        return program;
    }

	void OpenGLShaderProgramBuilder::clear()
	{
		for (GLuint shader : this->m_shaders) {
			glDeleteShader(shader);
		}
		this->m_shaders.clear();
	}

	OpenGLShaderProgramBuilder::~OpenGLShaderProgramBuilder()
	{
		this->clear();
	}

	bool OpenGLShaderProgramBuilder::addShader(const char* shaderSource, GLenum shaderType)
	{
		GLuint shader = glCreateShader(shaderType);

		const GLchar* source = shaderSource;
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		GLint isCompiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());
			glDeleteShader(shader);

			NS_CORE_ERROR("Failed to compile shader: \n{0}\n",  shaderSource);
			NS_CORE_ERROR("{0}", infoLog.data());

			return false;
		}
		this->m_shaders.emplace_back(shader);

		return true;
	}

	bool OpenGLShaderProgramBuilder::addShaderFromFile(const std::string& filePath, GLenum shaderType)
	{
		std::ifstream file(filePath);

		if (file.fail()) {
			NS_CORE_ERROR("Failed to open file: {0}\n", filePath);
			return false;
		}
		std::string shaderSource = "";
		std::string line;

		while (std::getline(file, line)) {
			shaderSource += line + "\n";
		}

		file.close();
		return this->addShader(shaderSource.c_str(), shaderType);
	}

}
