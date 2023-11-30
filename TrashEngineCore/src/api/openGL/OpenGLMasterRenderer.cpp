#include "nspch.h"

#include <imgui.h>
#include <api/openGL/deps/imgui_impl_glfw.h>
#include <api/openGL/deps/imgui_impl_opengl3.h>

#include <deps/glad/glad.h>
#include <TrashEngine/core/Time.h>
#include <TrashEngine/scene/PointLight.h>

#include "builder/OpenGLShaderProgramBuilder.h"
#include "OpenGLMasterRenderer.h"

namespace TrashEngine {
	
	OpenGLMasterRenderer::OpenGLMasterRenderer(glm::ivec2 renderSize) :
		m_renderSize(renderSize)
	{

		this->createDeferredPassFramebuffer();
		// Global uniform buffer (UBO)
		glCreateBuffers(1, &this->m_globalUniformBuffer);
		glNamedBufferData(this->m_globalUniformBuffer, sizeof(GlobalUBOData), nullptr, GL_DYNAMIC_DRAW);
		//glNamedBufferStorage(this->m_globalUniformBuffer, sizeof(GlobalUBOData), nullptr, GL_MAP_WRITE_BIT);

		this->m_staticModelRenderer = CreateScope<OpenGLStaticModelRenderer>();
		this->m_animatedModelRenderer = CreateScope<OpenGLAnimatedModelRenderer>();

		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/common/simpleQuad.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/common/simpleQuad.frag", GL_FRAGMENT_SHADER);
		this->m_simpleQuadProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_simpleQuadProgram->load(this->m_simpleQuadProgram->getUniformLocationFromName("textureSampler"), 0);

		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/PBRLighting.comp", GL_COMPUTE_SHADER);
		this->m_deferredCombineProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		
		// raw scene texture
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_rawSceneTexture);
		glTextureStorage2D(this->m_rawSceneTexture, 1, GL_RGBA32F, this->m_renderSize.x, this->m_renderSize.y);

		// point light creation
		glCreateBuffers(1, &this->m_pointLightsStorageBuffer);
		glNamedBufferStorage(this->m_pointLightsStorageBuffer, 4 * sizeof(uint32_t) + sizeof(PointLight) * MAX_POINT_LIGHTS, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/clusterCalc.comp", GL_COMPUTE_SHADER);
		this->m_clusterCalcProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();

		GLuint lightCalcBuffers[4];
		glCreateBuffers(4, lightCalcBuffers);
		this->m_clusterAABBBuffer = lightCalcBuffers[0];
		glNamedBufferStorage(this->m_clusterAABBBuffer, this->gridSizeX * this->gridSizeY * this->gridSizeZ * sizeof(glm::vec4) * 2, nullptr, GL_DYNAMIC_STORAGE_BIT);

		this->m_globalLightListBuffer = lightCalcBuffers[1];
		this->m_lightGridBuffer = lightCalcBuffers[2];
		this->m_globalIndexCountBuffer = lightCalcBuffers[3];
		glNamedBufferStorage(this->m_globalLightListBuffer, MAX_GLOBAL_LIGHTS * sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferStorage(this->m_lightGridBuffer, this->gridSizeX * this->gridSizeY * this->gridSizeZ * sizeof(uint32_t) * 2, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferStorage(this->m_globalIndexCountBuffer, sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);

		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/clusterLightCulling.comp", GL_COMPUTE_SHADER);
		this->m_clusterLightCullProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();

	}

	OpenGLMasterRenderer::~OpenGLMasterRenderer()
	{
		this->m_clusterLightCullProgram.reset();
		GLuint lightCalcBuffers[4] = {
			this->m_clusterAABBBuffer,
			this->m_globalLightListBuffer,
			this->m_lightGridBuffer,
			this->m_globalIndexCountBuffer
		};
		glDeleteBuffers(4, lightCalcBuffers);

		this->m_clusterCalcProgram.reset();

		glDeleteBuffers(1, &this->m_pointLightsStorageBuffer);

		glDeleteTextures(1, &this->m_rawSceneTexture);

		this->m_deferredCombineProgram.reset();
		glDeleteBuffers(1, &this->m_globalUniformBuffer);
		this->destroyDeferredPassFramebuffer();
	}

	void OpenGLMasterRenderer::renderFrame(Camera* camera, Scene* scene)
	{
		GLuint nextSceneTexture;
		Time time = Time::GetTime();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		this->prepareScene(scene);
		this->renderScene(camera, this->m_renderSize, this->m_rawSceneTexture);
		nextSceneTexture = this->m_rawSceneTexture;

		// draw to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, this->m_renderSize.x, this->m_renderSize.y);
		this->m_simpleQuadProgram->bind();
		glDisable(GL_DEPTH_TEST);
		glBindTextureUnit(0, nextSceneTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// ImGUI end frame
#ifdef NS_DEBUG
		ImGui::Begin("Performance");
		std::string drawTimeStr = "Draw Time: ";
		drawTimeStr.append(std::to_string(this->m_currentDrawTime * 1000));
		drawTimeStr.append(" ms");
		ImGui::Text(drawTimeStr.c_str());
		std::string FPSStr = "FPS: ";
		FPSStr.append(std::to_string(1.f / (this->m_currentDrawTime)));
		ImGui::Text(FPSStr.c_str());

		ImGui::End();
#endif // NS_DEBUG


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();

		Time currentTime = Time::GetTime();
		this->m_currentDrawTime = currentTime - time;

	}

	void OpenGLMasterRenderer::prepareScene(Scene* scene)
	{
		// prepare point lights
		auto view = scene->Reg().view<PointLight>();
		std::vector<PointLight> pointLights;
		for (auto [entity, light] : view.each()) {
			pointLights.emplace_back(light);
		}
		uint32_t numberOfPointLights = (uint32_t)pointLights.size();
		glNamedBufferSubData(this->m_pointLightsStorageBuffer, 0, sizeof(uint32_t), &numberOfPointLights);
		glNamedBufferSubData(this->m_pointLightsStorageBuffer, 4 * sizeof(uint32_t), numberOfPointLights * sizeof(PointLight), pointLights.data());

		// prepare Scene in renderers
		this->m_staticModelRenderer->prepareScene(scene);
		this->m_animatedModelRenderer->prepareScene(scene);
	}

	void OpenGLMasterRenderer::renderScene(Camera* camera, glm::ivec2 renderSize, GLuint drawTexture)
	{
		// camera information to camera buffer
		void* globalUBOMap = glMapNamedBuffer(this->m_globalUniformBuffer, GL_WRITE_ONLY);
		GlobalUBOData globalData{};
		globalData.projectionMatrix = camera->getProjectionMatrix();
		globalData.viewMatrix = camera->getViewMatrix();
		globalData.inverseProjectionMatrix = glm::inverse(camera->getProjectionMatrix());
		globalData.position = glm::vec4(camera->position, 0.f);
		memcpy_s(globalUBOMap, sizeof(GlobalUBOData), &globalData, sizeof(GlobalUBOData));
		glUnmapNamedBuffer(this->m_globalUniformBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->m_globalUniformBuffer);

		// start rendering on screen
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_deferredPassFramebuffer.handle);
		glViewport(0, 0, renderSize.x, renderSize.y);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		
		glEnable(GL_DEPTH_TEST);
		/// renderers draw
		this->m_staticModelRenderer->render();
		this->m_animatedModelRenderer->render();
		/// end renderers draw
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// point light buffer binding
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->m_pointLightsStorageBuffer);

		// clusterd size calculation
		// need to call when camera projection matrix has changed, but I just calc once in my case
		/*
		this->m_clusterCalcProgram->bind();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->m_clusterAABBBuffer);
		uint32_t sizeX = (uint32_t)std::ceilf(1280.f / (float)this->gridSizeX);
		this->m_clusterCalcProgram->load(this->m_clusterCalcProgram->getUniformLocationFromName("tileSizes"), glm::uvec4(this->gridSizeX, this->gridSizeY, this->gridSizeZ, sizeX));
		this->m_clusterCalcProgram->load(this->m_clusterCalcProgram->getUniformLocationFromName("screenDimensions"), glm::uvec2(1280, 720));
		this->m_clusterCalcProgram->load(this->m_clusterCalcProgram->getUniformLocationFromName("zValue"), glm::vec2(0.1f, 1000.f));
		glDispatchCompute(this->gridSizeX, this->gridSizeY, this->gridSizeZ);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// clustered light culling
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->m_globalLightListBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->m_lightGridBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, this->m_globalIndexCountBuffer);
		this->m_clusterLightCullProgram->bind();
		glDispatchCompute(1, 1, 6);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		*/


		// using compute shader to compute to
		this->m_deferredCombineProgram->bind();
		// glBindTexture();
		for (uint32_t i = 0; i < 5; i++) {
			glBindImageTexture(i, this->m_deferredPassFramebuffer.gBuffers[i], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		}
		glBindTextureUnit(0, this->m_deferredPassFramebuffer.depthBufferTexture);
		// You cant
		//glBindImageTexture(5, this->m_deferredPassFramebuffer.depthBufferTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
		glBindImageTexture(6, drawTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute((GLuint)ceil(renderSize.x / 32) + 1, (GLuint)ceil(renderSize.y / 32) + 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
		// render non deferred object like skybox sun ... etc

	}

	void OpenGLMasterRenderer::createDeferredPassFramebuffer()
	{
		glCreateFramebuffers(1, &this->m_deferredPassFramebuffer.handle);
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_deferredPassFramebuffer.depthBufferTexture);
		glCreateTextures(GL_TEXTURE_2D, 5, this->m_deferredPassFramebuffer.gBuffers);

		// setup depth buffer
		glTextureStorage2D(this->m_deferredPassFramebuffer.depthBufferTexture, 1, GL_DEPTH_COMPONENT32, this->m_renderSize.x, this->m_renderSize.y);
		glTextureParameteri(this->m_deferredPassFramebuffer.depthBufferTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(this->m_deferredPassFramebuffer.depthBufferTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glNamedFramebufferTexture(this->m_deferredPassFramebuffer.handle, GL_DEPTH_ATTACHMENT, this->m_deferredPassFramebuffer.depthBufferTexture, 0);
		

		// color buffer setup
		for (uint32_t i = 0; i < 5; i++) {
			glTextureStorage2D(this->m_deferredPassFramebuffer.gBuffers[i], 1, GL_RGBA32F, this->m_renderSize.x, this->m_renderSize.y);
			glTextureParameteri(this->m_deferredPassFramebuffer.depthBufferTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(this->m_deferredPassFramebuffer.depthBufferTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glNamedFramebufferTexture(this->m_deferredPassFramebuffer.handle, GL_COLOR_ATTACHMENT0 + i, this->m_deferredPassFramebuffer.gBuffers[i], 0);
		}

		GLenum drawBuffers[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4
		};
		glNamedFramebufferDrawBuffers(this->m_deferredPassFramebuffer.handle, 5, drawBuffers);

	}

	void OpenGLMasterRenderer::destroyDeferredPassFramebuffer()
	{
		glDeleteTextures(5, this->m_deferredPassFramebuffer.gBuffers);
		glDeleteTextures(1, &this->m_deferredPassFramebuffer.depthBufferTexture);
		glDeleteFramebuffers(1, &this->m_deferredPassFramebuffer.handle);
	}

}
