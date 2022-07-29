#pragma once

#include <entt/entt.hpp>

namespace JE {

    class Entity
    {
    public:
        Entity(entt::entity entityId, entt::registry& registry) : m_EntityID(entityId), m_Registry(registry) {}

        template<typename T, typename... Args>
        auto AddComponent(Args&&... args) -> decltype(auto)
        {
            return m_Registry.emplace_or_replace<T>(m_EntityID, std::forward<Args>(args)...);
        }

        template<typename T, typename... Other>
        auto DeleteComponent() -> size_t
        {
            return m_Registry.remove<T, Other...>(m_EntityID);
        }

        template<typename... Args>
        auto GetComponent() -> decltype(auto)
        {
            return m_Registry.get<Args...>(m_EntityID);
        }

        template<typename T>
        auto HasComponent() -> bool
        {
            return m_Registry.try_get<T>(m_EntityID) != nullptr;
        }

    private:
        entt::entity    m_EntityID;
        entt::registry& m_Registry;
    };

}// namespace JE