#pragma once

#include <string>

namespace JE {

    class IEvent;

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class ILayer
    {
    public:
        explicit ILayer(const std::string_view& name) : m_DebugName(name) {}
        virtual ~ILayer() = default;

        virtual void OnCreate()  = 0;
        virtual void OnDestroy() = 0;

        virtual void OnUpdate()      = 0;
        virtual void OnImGuiRender() = 0;

        virtual void OnEvent(IEvent& event) = 0;

        [[nodiscard]] inline auto DebugName() const -> const std::string& { return m_DebugName; }

    protected:
        const std::string m_DebugName;
    };

}// namespace JE