#include "nspch.h"
#include <TrashEngine/scene/component/TerrainComponent.h>
#include "OpenGLTerrainRenderer.h"
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>


namespace TrashEngine {

	struct TerrainVertexData
	{
		glm::vec3 position;
	};

	OpenGLTerrainRenderer::OpenGLTerrainRenderer()
	{
		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/terrain/shader.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/terrain/shader.tesc", GL_TESS_CONTROL_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/terrain/shader.tese", GL_TESS_EVALUATION_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/terrain/shader.frag", GL_FRAGMENT_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		this->m_shader->getUniformLocationFromName("u_heightIntensity");
		this->m_shader->getUniformLocationFromName("u_position");
		this->m_shader->getUniformLocationFromName("u_heightMap");
		this->m_shader->getUniformLocationFromName("u_normalMap");
		this->m_shader->loadInt("u_heightMap", 0);
		this->m_shader->loadInt("u_normalMap", 1);

		// setup up terrain vertex buffer
		const uint32_t divison = 32;
		uint32_t count = divison * divison;
		this->m_indicesCount = 4 * (divison - 1) * (divison - 1);
		float side = 800.f / (float)(divison - 1);
		std::vector<TerrainVertexData> vertices(count);
		std::vector<uint32_t> indices(this->m_indicesCount);
		uint32_t col, row;
		uint32_t vertexPointer = 0;
		uint32_t pointer = 0;
		for (row = 0; row < divison; row++) {
			for (col = 0; col < divison; col++) {
				vertices[vertexPointer].position = glm::vec3(col * side, 0, row * side);
				//vertices[vertexPointer].texCoord = glm::vec2(col / (float)(divison - 1), row / (float)(divison - 1));
				vertexPointer++;
			}
		}

		for (row = 0; row < divison - 1; row++) {
			for (col = 0; col < divison - 1; col++) {
				int topLeft = (row * divison) + col;
				int topRight = topLeft + 1;
				int bottomLeft = ((row + 1) * divison) + col;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomRight;
				indices[pointer++] = bottomLeft;
			}
		}

		glCreateVertexArrays(1, &this->m_meshVaoHandle);
		GLuint buffers[2];
		glCreateBuffers(2, buffers);
		this->m_dataVertexBuffer = buffers[0];
		this->m_indexVertexBuffer = buffers[1];

		glNamedBufferData(this->m_dataVertexBuffer, vertices.size() * sizeof(TerrainVertexData), vertices.data(), GL_STATIC_DRAW);
		glNamedBufferData(this->m_indexVertexBuffer, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		glVertexArrayVertexBuffer(this->m_meshVaoHandle, 0, this->m_dataVertexBuffer, 0, sizeof(TerrainVertexData));
		glVertexArrayElementBuffer(this->m_meshVaoHandle, this->m_indexVertexBuffer);

		glEnableVertexArrayAttrib(this->m_meshVaoHandle, 0);
		glVertexArrayAttribBinding(this->m_meshVaoHandle, 0, 0);
		glVertexArrayAttribFormat(this->m_meshVaoHandle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(TerrainVertexData, position));

	}

	OpenGLTerrainRenderer::~OpenGLTerrainRenderer()
	{
		GLuint buffers[2] = { this->m_dataVertexBuffer, this->m_indexVertexBuffer };
		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &this->m_meshVaoHandle);
	}

	void OpenGLTerrainRenderer::prepareScene(Scene* scene)
	{
		// frustum culling
		this->m_terrains.clear();
		auto view = scene->Reg().view<TerrainComponent>();
		for (auto [entity, terrain] : view.each()) {
			Ref<OpenGLTerrain> ogTerrain = StaticCastRef<OpenGLTerrain>(terrain.terrain);
			this->m_terrains.push_back(ogTerrain);
		}
	}

	void OpenGLTerrainRenderer::render()
	{
		this->m_shader->bind();
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glBindVertexArray(this->m_meshVaoHandle);
		// for each terrain
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (auto terrain : this->m_terrains) {
			// bind terrain textures (normal map, height map, blend map, terrain textures)
			terrain->getHeightMapTexture()->bindUnit(0);
			terrain->getNormalMapTexture()->bindUnit(1);
			this->m_shader->loadFloat("u_heightIntensity", terrain->getHeightIntensity());
			this->m_shader->loadIVec2("u_position", terrain->getPosition());
			glDrawElements(GL_PATCHES, this->m_indicesCount, GL_UNSIGNED_INT, 0);
		}
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}
