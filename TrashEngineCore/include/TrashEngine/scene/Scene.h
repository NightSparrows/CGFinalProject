#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/deps/entt/entt.hpp>

namespace TrashEngine {

	class Entity;

	class Scene
	{
		friend class Entity;
	public:

		NS_API Entity createEntity(const std::string& name = std::string());

		NS_API void removeEntity(Entity entity);

		NS_API Scene() = default;
		Scene(const Scene&) = delete;
		void operator=(const Scene&) = delete;

		entt::registry& Reg() { return this->m_registry; }
		const entt::registry& Reg() const { return this->m_registry; }

	private:
		entt::registry m_registry;
	};

}
