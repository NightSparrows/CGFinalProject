#include "nspch.h"
#include <core/Logger.h>
#include <deps/glad/glad.h>
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>
#include "OpenGLParticleRenderer.h"
#include <TrashEngine/utils/Transform.h>
#include <TrashEngine/deps/glm/gtc/quaternion.hpp>
#include <TrashEngine/scene/component/ParticleEmitterComponent.h>

namespace TrashEngine {

	static Ref<OpenGLParticle> testPart;
	OpenGLParticleRenderer::OpenGLParticleRenderer()
	{
		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/particle/shader.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/particle/shader.frag", GL_FRAGMENT_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_shader->getUniformLocationFromName("u_particleTexture");
		this->m_shader->getUniformLocationFromName("u_numberOfRow");
		this->m_shader->loadInt("u_particleTexture", 0);

		
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/particle/timeUpdate.comp", GL_COMPUTE_SHADER);
		this->m_updateComputeProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_updateComputeProgram->getUniformLocationFromName("u_particleCount");
		this->m_updateComputeProgram->getUniformLocationFromName("u_deltaTime");

		glCreateVertexArrays(1, &this->m_vaoHandle);

		glCreateBuffers(1, &this->m_instanceBuffer);
		glNamedBufferData(this->m_instanceBuffer, sizeof(ParticleInstanceData) * MAX_PARTICLE_COUNT, nullptr, GL_STREAM_DRAW);

		glCreateBuffers(1, &this->m_dataBuffer);
		glNamedBufferData(this->m_dataBuffer, sizeof(ParticleData) * MAX_PARTICLE_COUNT, nullptr, GL_STREAM_DRAW);

		glVertexArrayVertexBuffer(this->m_vaoHandle, 0, this->m_instanceBuffer, 0, sizeof(ParticleInstanceData));
		glVertexArrayBindingDivisor(this->m_vaoHandle, 0, 1);

		// transformationMatrix
		glEnableVertexArrayAttrib(this->m_vaoHandle, 0);
		glVertexArrayAttribBinding(this->m_vaoHandle, 0, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(ParticleInstanceData, position));
		glEnableVertexArrayAttrib(this->m_vaoHandle, 1);
		glVertexArrayAttribBinding(this->m_vaoHandle, 1, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 1, 1, GL_FLOAT, GL_FALSE, offsetof(ParticleInstanceData, rotation));
		glEnableVertexArrayAttrib(this->m_vaoHandle, 2);
		glVertexArrayAttribBinding(this->m_vaoHandle, 2, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 2, 1, GL_FLOAT, GL_FALSE, offsetof(ParticleInstanceData, scale));
		// texOffsets
		glEnableVertexArrayAttrib(this->m_vaoHandle, 3);
		glVertexArrayAttribBinding(this->m_vaoHandle, 3, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 3, 4, GL_FLOAT, GL_FALSE, offsetof(ParticleInstanceData, texOffsets));
		// blend
		glEnableVertexArrayAttrib(this->m_vaoHandle, 4);
		glVertexArrayAttribBinding(this->m_vaoHandle, 4, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 4, 1, GL_FLOAT, GL_FALSE, offsetof(ParticleInstanceData, blend));


		testPart = CreateRef<OpenGLParticle>("res/particle/particleAtlas.png", 4);
	}
	OpenGLParticleRenderer::~OpenGLParticleRenderer()
	{
		glDeleteBuffers(1, &this->m_dataBuffer);
		glDeleteBuffers(1, &this->m_instanceBuffer);

		glDeleteVertexArrays(1, &this->m_vaoHandle);
	}

	void OpenGLParticleRenderer::prepareScene(Scene* scene)
	{
		// add particle
		auto view = scene->Reg().view<ParticleEmitterComponent>();
		for (auto [entity, particleEmitter] : view.each()) {
			Ref<OpenGLParticle> opParticle = StaticCastRef<OpenGLParticle>(particleEmitter.particle);
			auto& list = this->m_particles[opParticle];
			for (auto& startInfo : particleEmitter.startInfos) {
				auto& pData = list.emplace_back();
				pData.position = startInfo.position;
				pData.velocity = startInfo.velocity;
				pData.rotation = startInfo.rotation;
				pData.scale = startInfo.scale.x;			// TODO make it 2D
				pData.elaspedTime = 0;
				pData.lifeLength = startInfo.lifeLength;
				pData.gravityEffect = startInfo.gravityEffect;
			}
			particleEmitter.startInfos.clear();
		}
	}

	static float time = 0;

	glm::vec2 calcTextureOffset(uint32_t index, uint32_t numberOfRow)
	{
		uint32_t column = index % numberOfRow;
		uint32_t row = index / numberOfRow;
		glm::vec2 offset;
		offset.x = (float)column / numberOfRow;
		offset.y = (float)row / numberOfRow;
		return offset;
	}

	void OpenGLParticleRenderer::update(Camera* camera, Time deltaTime)
	{
		time += deltaTime;

		uint32_t instanceOffset = 0;
		for (auto& [particle, list] : this->m_particles) {
			std::vector<ParticleInstanceData> instanceList;
			std::for_each(std::execution::par, list.begin(), list.end(), [particle, deltaTime, &instanceList](ParticleData& data) {

				float deltaTimeAsSec = deltaTime.asSecond();
				data.position += data.velocity * deltaTimeAsSec;
				data.velocity.y -= 10.f * data.gravityEffect * deltaTimeAsSec;
				data.elaspedTime += deltaTime.asSecond();

				});
			std::erase_if(list, [](ParticleData& data) {
				return data.lifeLength < data.elaspedTime;
				});

			std::ranges::iota_view indexes((size_t)0, list.size());
			instanceList.resize(list.size());
			std::for_each(std::execution::par, indexes.begin(), indexes.end(), [&list, particle, deltaTime, &instanceList](size_t i) {

				ParticleData& data = list[i];
				// update texture coord
				float lifeFactor = data.elaspedTime / data.lifeLength;
				uint32_t stageCount = particle->getNumberOfRow() * particle->getNumberOfRow();
				float atlasProgression = lifeFactor * (float)stageCount;
				uint32_t index1 = (uint32_t)std::floor(atlasProgression);
				uint32_t index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

				bool isDead = data.lifeLength < data.elaspedTime;
				if (!isDead) {
					auto& instanceData = instanceList[i];
					instanceData.position = data.position;
					instanceData.rotation = data.rotation;
					instanceData.scale = data.scale;
					instanceData.blend = std::fmod(atlasProgression, 1.f);
					glm::vec2 texOffset1 = calcTextureOffset(index1, particle->getNumberOfRow());
					glm::vec2 texOffset2 = calcTextureOffset(index2, particle->getNumberOfRow());
					instanceData.texOffsets = glm::vec4(texOffset1.x, texOffset1.y, texOffset2.x, texOffset2.y);
				}
				});

			std::sort(instanceList.begin(), instanceList.end(), [&camera](const ParticleInstanceData& a, const ParticleInstanceData& b) {
				return glm::length(a.position - camera->position) > glm::length(b.position - camera->position);
				});

			if (instanceOffset + (uint32_t)instanceList.size() > MAX_PARTICLE_COUNT)
				break;

			glNamedBufferSubData(this->m_instanceBuffer, instanceOffset * sizeof(ParticleInstanceData), instanceList.size() * sizeof(ParticleInstanceData), instanceList.data());
			instanceOffset += (uint32_t)instanceList.size();
		}

		this->m_currentParticleCount = instanceOffset;
	}

	void OpenGLParticleRenderer::render()
	{
		this->m_shader->bind();
		glBindVertexArray(this->m_vaoHandle);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		uint32_t instanceOffset = 0;
		for (auto& [particle, list] : this->m_particles) {
			if (particle->getTexture() != nullptr) {
				particle->getTexture()->bindUnit(0);
				this->m_shader->loadFloat("u_numberOfRow", particle->getNumberOfRow());
			}
			if (instanceOffset + (uint32_t)list.size() > MAX_PARTICLE_COUNT)
				break;
			// load particle attribute to shader
			//glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)list.size());
			glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)list.size(), instanceOffset);
			instanceOffset += (uint32_t)list.size();
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
	}

}
