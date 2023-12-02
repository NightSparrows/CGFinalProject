#pragma once

#include <deps/glad/glad.h>

#include "OpenGLRenderer.h"
#include <api/openGL/objects/OpenGLShaderProgram.h>
#include <api/openGL/model/terrain/OpenGLTerrain.h>

namespace TrashEngine {

	class OpenGLTerrainRenderer : public OpenGLRenderer
	{
	public:
		OpenGLTerrainRenderer();
		~OpenGLTerrainRenderer();

		void prepareScene(Scene* scene) override;

		void render() override;


	private:

		Scope<OpenGLShaderProgram> m_shader;

		GLuint m_meshVaoHandle;
		GLuint m_indexVertexBuffer;
		GLuint m_dataVertexBuffer;

		uint32_t m_indicesCount;

		std::vector<Ref<OpenGLTerrain>> m_terrains;

	};

}
