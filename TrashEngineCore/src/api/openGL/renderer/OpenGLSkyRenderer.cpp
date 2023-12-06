#include "nspch.h"
#include "OpenGLSkyRenderer.h"

#include <TrashEngine/loader/ModelLoader.h>
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>

namespace TrashEngine {

	OpenGLSkyRenderer::OpenGLSkyRenderer()
	{
		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/atmosphereSky/shader.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/atmosphereSky/shader.frag", GL_FRAGMENT_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		this->m_shader->getUniformLocationFromName("u_sunPosition");
		this->m_shader->getUniformLocationFromName("u_viewSamples");
		this->m_shader->getUniformLocationFromName("u_lightSamples");
		this->m_shader->getUniformLocationFromName("u_sunIntensity");
		this->m_shader->getUniformLocationFromName("u_radiusPlanet");
		this->m_shader->getUniformLocationFromName("u_radiusAtmoSphere");
		this->m_shader->getUniformLocationFromName("u_rayleighCoeff");
		this->m_shader->getUniformLocationFromName("u_minScatteringCoeff");
		this->m_shader->getUniformLocationFromName("u_rayleighScaleHeight");
		this->m_shader->getUniformLocationFromName("u_mieScaleHeight");
		this->m_shader->getUniformLocationFromName("u_mieScatterDir");
		this->m_shader->getUniformLocationFromName("u_seeHeight");
		this->m_shader->loadInt("u_viewSamples", 16);
		this->m_shader->loadInt("u_lightSamples", 8);
		this->m_shader->loadFloat("u_sunIntensity", 20.f);
		this->m_shader->loadVec3("u_rayleighCoeff", glm::vec3(5.8e-3f, 13.5e-3f, 33.1e-3f));
		this->m_shader->loadFloat("u_minScatteringCoeff", 21e-3f);
		this->m_shader->loadFloat("u_rayleighScaleHeight", 7.994f);
		this->m_shader->loadFloat("u_mieScaleHeight", 1.2f);
		this->m_shader->loadFloat("u_mieScatterDir", 0.888f);
		glm::mat4 transMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1000.f));
		this->m_shader->load(this->m_shader->getUniformLocationFromName("u_transformationMatrix"), transMatrix);

		this->m_sphereModel = CreateScope<OpenGLStaticModel>();
		auto modelData = ModelLoader::LoadStaticModelFromFile("assets/TrashEngine/model/sphere.obj", "");
		this->m_sphereModel->loadData(modelData.get());

	}

	OpenGLSkyRenderer::~OpenGLSkyRenderer()
	{
	}

	void OpenGLSkyRenderer::prepareScene(Scene* scene)
	{
	}

	void OpenGLSkyRenderer::render()
	{
		this->m_shader->bind();
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);

		this->m_shader->loadFloat("u_radiusPlanet", this->m_planetRadius);
		this->m_shader->loadFloat("u_radiusAtmoSphere", this->m_atmosphereRadius);
		this->m_shader->loadFloat("u_seeHeight", this->m_seeHeight);
		this->m_shader->loadVec3("u_sunPosition", this->m_sunDirection);
		this->m_sphereModel->bind();
		glDrawElements(GL_TRIANGLES, this->m_sphereModel->getMeshes()[0].indexCount, GL_UNSIGNED_INT, 0);

		glDepthMask(GL_TRUE);
	}

	void OpenGLSkyRenderer::setSunAngle(float angle)
	{
		this->m_sunAngle = glm::radians(angle);
		this->m_sunDirection.x = 0;
		this->m_sunDirection.y = glm::sin(this->m_sunAngle);
		this->m_sunDirection.z = -glm::cos(this->m_sunAngle);
		this->calcSunColor();
	}

	glm::vec2 raySphereIntersection(glm::vec3 o, glm::vec3 d, float r)
	{
		float a = dot(d, d);
		float b = 2.0 * dot(d, o);
		float c = dot(o, o) - r * r;

		float delta = b * b - 4.0 * a * c;

		// Roots not found
		if (delta < 0.0) {
			// TODO
			return glm::vec2(1e5, -1e5);
		}

		float sqrtDelta = sqrt(delta);

		return glm::vec2((-b - sqrtDelta) / (2.0 * a),
			(-b + sqrtDelta) / (2.0 * a));
	}

	glm::vec3 OpenGLSkyRenderer::computeSkyColor(glm::vec3 ray, glm::vec3 origin)
	{
		glm::vec3 sunDir = glm::normalize(this->m_sunDirection);

		glm::vec2 t = raySphereIntersection(origin, ray, this->m_atmosphereRadius);

		if (t.x > t.y) {
			return glm::vec3(0.0);
		}

		t.y = glm::min(t.y, raySphereIntersection(origin, ray, this->m_planetRadius).x);
		float segmentLen = (t.y - t.x) / float(16/* view sample */);

		// TODO t min
		float tCurrent = 0.0;

		glm::vec3 sum_R = glm::vec3(0);
		glm::vec3 sum_M = glm::vec3(0);

		// Optical depth 
		float optDepth_R = 0.0;
		float optDepth_M = 0.0;

		// Mu: the cosine angle between the sun and ray direction
		float mu = dot(ray, sunDir);
		float mu_2 = mu * mu;

		//--------------------------------
		// Rayleigh and Mie Phase functions
		float phase_R = 3.0 / (16.0 * 3.1415926535897932384626433832795) * (1.0 + mu_2);

		float g_2 = 0.888f * 0.888f;
		float phase_M = 3.0 / (8.0 * 3.1415926535897932384626433832795) *
			((1.0 - g_2) * (1.0 + mu_2)) /
			((2.0 + g_2) * pow(1.0 + g_2 - 2.0 * 0.888f * mu, 1.5));
		// Sample along the view ray
		for (int i = 0; i < 16; ++i) {
			// Middle point of the sample position
			glm::vec3 vSample = origin + ray * (tCurrent + segmentLen * 0.5f);

			// Height of the sample above the planet
			float height = length(vSample) - this->m_planetRadius;

			// Optical depth for Rayleigh and Mie scattering for current sample
			float h_R = exp(-height / 7.994f) * segmentLen;
			float h_M = exp(-height / 1.2f) * segmentLen;
			optDepth_R += h_R;
			optDepth_M += h_M;

			//--------------------------------
			// Secondary - light ray
			float segmentLenLight = raySphereIntersection(vSample, sunDir, this->m_atmosphereRadius).y / float(8);
			float tCurrentLight = 0.0;

			// Light optical depth 
			float optDepthLight_R = 0.0;
			float optDepthLight_M = 0.0;

			// Sample along the light ray
			for (int j = 0; j < 8; ++j) {
				// Position of the light ray sample
				glm::vec3 lSample = vSample + sunDir * (tCurrentLight + segmentLenLight * 0.5f);
				// Height of the light ray sample
				float heightLight = glm::length(lSample) - this->m_planetRadius;

				// TODO check sample above the ground

				optDepthLight_R += exp(-heightLight / 7.994f) * segmentLenLight;
				optDepthLight_M += exp(-heightLight / 1.2f) * segmentLenLight;

				// Next light sample
				tCurrentLight += segmentLenLight;
			}
			// TODO check sample above ground

			// Attenuation of the light for both Rayleigh and Mie optical depth
			//  Mie extenction coeff. = 1.1 of the Mie scattering coeff.
			glm::vec3 att = exp(-(glm::vec3(5.8e-3f, 13.5e-3f, 33.1e-3f) * (optDepth_R + optDepthLight_R) +
				21e-3f * 1.1f * (optDepth_M + optDepthLight_M)));
			// Accumulate the scattering 
			sum_R += h_R * att;
			sum_M += h_M * att;

			// Next view sample
			tCurrent += segmentLen;
		}

		return 20.f * (sum_R * glm::vec3(5.8e-3f, 13.5e-3f, 33.1e-3f) * phase_R + sum_M * 21e-3f * phase_M);
	}

	void OpenGLSkyRenderer::calcSunColor()
	{
		glm::vec3 viewPos = glm::vec3(0, this->m_seeHeight, 0);
		glm::vec3 sunPos = this->m_sunDirection * 1000.f;
		this->m_sunColor = this->computeSkyColor(glm::normalize(sunPos - viewPos), viewPos);
	}

}
