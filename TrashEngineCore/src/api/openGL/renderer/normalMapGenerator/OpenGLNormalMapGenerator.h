#pragma once

#include <TrashEngine/core/Base.h>
#include <api/openGL/objects/OpenGLTexture.h>
#include <api/openGL/objects/OpenGLShaderProgram.h>

namespace TrashEngine {

	class OpenGLNormalMapGenerator
	{
	public:
		OpenGLNormalMapGenerator();
		~OpenGLNormalMapGenerator();

		bool generateNormalMapFromHeight(OpenGLTexture* heightMap, OpenGLTexture* normalMap);

	protected:

		Scope<OpenGLShaderProgram> m_shader;


	};

}
