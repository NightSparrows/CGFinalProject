#include "nspch.h"
#include <TrashEngine/scene/Scene.h>
#include <TrashEngine/scene/Entity.h>
//#include <TrashEngine/scene/component/TagComponent.h>
#include <TrashEngine/scene/component/TransformComponent.h>


namespace TrashEngine {

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { this->m_registry.create(), this };
		//auto& tag = entity.addComponent<TagComponent>();
		entity.addComponent<TransformComponent>();
		//tag.name = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::removeEntity(Entity entity)
	{
		this->m_registry.destroy(entity);
	}

}
