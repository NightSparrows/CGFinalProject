
#include <TrashEngine/TrashEngine.h>
#include <TrashEngine/scene/component/StaticModelComponent.h>
#include "FinalProjectState.h"

FinalProjectState::FinalProjectState(TrashEngine::GameEngine* engine) : 
	GameState("finalState"), m_engine(engine)
{
}

void FinalProjectState::onEvent(TrashEngine::Event& evnt)
{
	TrashEngine::EventDispatcher dispatcher(evnt);

	dispatcher.dispatch<TrashEngine::WindowCloseEvent>([this](TrashEngine::WindowCloseEvent& event) {
		this->m_engine->stop();
		return true;
		});
}

TrashEngine::Ref<TrashEngine::MasterRenderer> masterRenderer;
TrashEngine::Ref<TrashEngine::StaticModel> testModel;
TrashEngine::Ref<TrashEngine::Scene> scene;
TrashEngine::Entity testEntity;
TrashEngine::Entity testEntity2;
TrashEngine::Camera camera(1280.f / 720.f, glm::radians(70.f), 0.1f, 1000.f);
TrashEngine::Entity testLightEntity;

void FinalProjectState::onInit()
{
	camera.position.z += 20.f;
	camera.updateViewMatrix();

	scene = TrashEngine::CreateRef<TrashEngine::Scene>();

	masterRenderer = this->m_engine->getGraphicsContext()->createMasterRenderer(glm::ivec2(1280, 720));
	TrashEngine::Ref<TrashEngine::StaticModelData> modelData = TrashEngine::CreateRef<TrashEngine::StaticModelData>();
	modelData->vertices.resize(4);
	float size = 50.f;
	modelData->vertices[0].position = { -size, size, 0 };
	modelData->vertices[0].texCoord = { 0, 0 };
	modelData->vertices[0].normal = { 0, 0, 1.f };
	modelData->vertices[0].tangent = { 0, 1.f, 0 };
	modelData->vertices[1].position = { -size, -size, 0 };
	modelData->vertices[1].texCoord = { 0, 1.f };
	modelData->vertices[1].normal = { 0, 0, 1.f };
	modelData->vertices[1].tangent = { 0, 1.f, 0 };
	modelData->vertices[2].position = { size, -size, 0 };
	modelData->vertices[2].texCoord = { 1.f, 1.f };
	modelData->vertices[2].normal = { 0, 0, 1.f };
	modelData->vertices[2].tangent = { 0, 1.f, 0 };
	modelData->vertices[3].position = { size, size, 0 };
	modelData->vertices[3].texCoord = { 1.f, 0 };
	modelData->vertices[3].normal = { 0, 0, 1.f };
	modelData->vertices[3].tangent = { 0, 1.f, 0 };
	modelData->indices = { 0, 1, 3, 3, 1, 2 };
	modelData->materials.resize(1);
	modelData->materials[0].diffuseColor = glm::vec3(1, 0, 1);
	modelData->materials[0].diffuseTexture = "res/wallDiffuse.png";
	modelData->materials[0].normalTexture = "res/wallNormal.png";
	modelData->materials[0].diffuse = glm::vec3(1);
	modelData->materials[0].ambient = glm::vec3(0);
	modelData->materials[0].specular = glm::vec3(0.5);
	modelData->materials[0].shininess = 10.f;
	modelData->materials[0].reflectivity = 1.f;
	modelData->meshes.resize(1);
	modelData->meshes[0].indexCount = 6;
	modelData->meshes[0].materialIndex = 0;

	testModel = this->m_engine->getGraphicsContext()->createStaticModel();
	testModel->loadData(modelData.get());

	testEntity = scene->createEntity();
	auto& modelCom = testEntity.addComponent<TrashEngine::StaticModelComponent>();
	modelCom.model = testModel;
	testEntity.getComponent<TrashEngine::TransformComponent>().transform.setPosition(glm::vec3(0, 0, -10));

	testLightEntity = scene->createEntity("Light");
	auto& lightCom = testLightEntity.addComponent<TrashEngine::PointLight>();
	lightCom.position = glm::vec3(5, 5, 10);
	lightCom.enabled = 1.f;
	lightCom.color = glm::vec3(5.f, 5.f, 5.f);
	lightCom.attenuation.x = 1.f;
	lightCom.attenuation.y = 0.009f;
	lightCom.attenuation.z = 0.0032f;
	lightCom.attenuation.w = 20.f;

	
	auto testLightEntity2 = scene->createEntity("Light2");
	auto& lightCom2 = testLightEntity2.addComponent<TrashEngine::PointLight>();
	lightCom2.position = glm::vec3(5, 10, 10);
	lightCom2.enabled = 1.f;
	lightCom2.color = glm::vec3(1.f, 0.f, 0.f);
	lightCom2.attenuation.x = 1.f;
	lightCom2.attenuation.y = 0.09f;
	lightCom2.attenuation.z = 0.032f;
	lightCom2.attenuation.w = 20.f;
	
	/*
	for (uint32_t y = 0; y < 20; y++) {
		for (uint32_t x = 0; x < 20; x++) {
			auto testLightEnt = scene->createEntity("testLight");
			auto& lightC = testLightEnt.addComponent<TrashEngine::PointLight>();
			lightC.position = glm::vec3((float)x * 5.f - 50.f, (float)y * 5.f - 50.f, 2);
			lightC.enabled = 1.f;
			lightC.color = glm::vec3((float)x / 20.f, y / 20.f, 0);
			lightC.attenuation.x = 1.f;
			lightC.attenuation.y = 0.09f;
			lightC.attenuation.z = 0.032f;
			lightC.attenuation.w = 20.f;

		}
	}*/

	testEntity2 = scene->createEntity("test entity2");

	//auto testAnimatedModelData = TrashEngine::ModelLoader::LoadAnimatedModelFromFile("res/thinMatrixModel/model.dae", "res/thinMatrixModel");
	auto testAnimatedModelData = TrashEngine::ModelLoader::LoadAnimatedModelFromFile("res/archer/dae/archer_walk.dae", "res/archer/textures");
	testAnimatedModelData->materials[0].diffuseTexture = "res/archer/textures/Archer.png";
	auto& modelC = testEntity2.addComponent<TrashEngine::AnimatedModelComponent>();
	modelC.model = this->m_engine->getGraphicsContext()->createAnimatedModel();
	modelC.model->loadData(testAnimatedModelData.get());
	auto& testEnt2Transform = testEntity2.getComponent<TrashEngine::TransformComponent>();
	testEnt2Transform.transform.setPosition(glm::vec3(-25, 0, 0));
	testEnt2Transform.transform.rotate(glm::vec3(1, 0, 0), -90.f);
	auto& animatorCom = testEntity2.addComponent<TrashEngine::AnimatedModelAnimatorComponent>();
	animatorCom.animator = this->m_engine->getGraphicsContext()->createAnimatedModelAnimator(modelC.model);
	//animatorCom.animator->addAnimation("walk", TrashEngine::ModelLoader::LoadAnimatedModelAnimationFromFile("res/thinMatrixModel/model.dae"));
	animatorCom.animator->addAnimation("walk", TrashEngine::ModelLoader::LoadAnimatedModelAnimationFromFile("res/archer/dae/archer_walk.dae"));
	animatorCom.animator->addAnimation("idle", TrashEngine::ModelLoader::LoadAnimatedModelAnimationFromFile("res/archer/dae/archer_hit.dae"));
	animatorCom.animator->play("walk");
	animatorCom.animator->play("idle");
}

float testTime = 0;
void FinalProjectState::onUpdate(TrashEngine::Time delta)
{
	auto& animatorCom = testEntity2.getComponent<TrashEngine::AnimatedModelAnimatorComponent>();
	animatorCom.animator->update(delta);

	auto& light = testLightEntity.getComponent<TrashEngine::PointLight>();

	light.position.x = sin(testTime) * 50.f;
	testTime += delta;

	//auto& entTransform = testEntity.getComponent<TrashEngine::TransformComponent>().transform;
	
	//entTransform.setRotation(glm::rotate(entTransform.getRotation(), delta * 1.f, glm::vec3(0, 1, 0)));

	const float speed = 25.f;
	glm::vec3 vector = glm::vec3(0);
	if (this->m_engine->getKeyboard()->isKeyDown(TrashEngine::Key::W)) {
		vector.z -= speed;
	}
	if (this->m_engine->getKeyboard()->isKeyDown(TrashEngine::Key::S)) {
		vector.z += speed;
	}
	if (this->m_engine->getKeyboard()->isKeyDown(TrashEngine::Key::A)) {
		vector.x -= speed;
	}
	if (this->m_engine->getKeyboard()->isKeyDown(TrashEngine::Key::D)) {
		vector.x += speed;
	}
	if (this->m_engine->getKeyboard()->isKeyDown(TrashEngine::Key::Space)) {
		vector.y += speed;
	}
	if (this->m_engine->getKeyboard()->isKeyDown(TrashEngine::Key::LeftShift)) {
		vector.y -= speed;
	}
	vector = camera.rotation * vector;
	camera.position += vector * delta.asSecond();
	camera.updateViewMatrix();
}

void FinalProjectState::onCleanUp()
{
	testModel.reset();
	masterRenderer.reset();
}

void FinalProjectState::onRender(TrashEngine::Time time)
{
	masterRenderer->renderFrame(&camera, scene.get());
}
