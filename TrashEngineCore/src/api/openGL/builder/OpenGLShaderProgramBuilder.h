#pragma once

#include <vector>
#include <api/openGL/objects/OpenGLShaderProgram.h>

namespace TrashEngine {

	class OpenGLShaderProgramBuilder
	{
	public:
		~OpenGLShaderProgramBuilder();

		bool addShader(const char* shaderSource, GLenum shaderType);

		bool addShaderFromFile(const std::string& filePath, GLenum shaderType);


		/// <summary>
		/// create new shader program
		/// </summary>
		/// <returns></returns>
		OpenGLShaderProgram* build();

		void clear();

	private:

		std::vector<GLuint> m_shaders;

	};

}
