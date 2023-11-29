#pragma once

#include "Scene.h"

namespace TrashEngine {

	class Entity
	{
	public:
		Entity(entt::entity handle, Scene* scene) :
			m_handle(handle), m_scene(scene) { }

		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			return this->m_scene->m_registry.emplace<T>(this->m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		void removeComponent()
		{
			this->m_scene->m_registry.remove<T>(this->m_handle);
		}

		template<typename T>
		T& getComponent()
		{
			return this->m_scene->m_registry.get<T>(this->m_handle);
		}

		template<typename T>
		bool hasComponent()
		{
			return this->m_scene->m_registry.any_of<T>(this->m_handle);
		}

		operator entt::entity() { return this->m_handle; }
		operator entt::entity() const { return this->m_handle; }

		Entity() = default;

	private:
		entt::entity m_handle{ entt::null };

		Scene* m_scene{ nullptr };

	};

}
