#include "InputController.hpp"


#include <iterator>// for end
#include <utility>// for pair

namespace JE {

    InputController* InputController::s_InputControllerInstance = nullptr;// NOLINT

    InputController::InputController()
    {
        ASSERT(!s_InputControllerInstance, "InputController instance already exists");
        ASSERT(IPlatform::Get().Initialized(), "Backend needs to be initialized before using this class");

        s_InputControllerInstance = this;
    }
    InputController::~InputController() { s_InputControllerInstance = nullptr; }

    void InputController::OnEvent(IEvent& event)
    {
        if (event.Category() != EventCategory::Keyboard && event.Category() != EventCategory::Mouse) {
            Logger::CoreLogger().error("InputController received unknown event type");
            return;
        }
        EventDispatcher dispatcher{ event };

        dispatcher.Dispatch<EventType::KeyPress>([&](const IEvent& evnt) {
            const auto& keyPressEvent = static_cast<const KeyPressEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            m_KeyMap[keyPressEvent.Key()] = true;

            return true;
        });

        dispatcher.Dispatch<EventType::KeyRelease>([&](const IEvent& evnt) {
            const auto& keyReleaseEvent =
            static_cast<const KeyReleaseEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            m_KeyMap[keyReleaseEvent.Key()] = false;

            return true;
        });

        dispatcher.Dispatch<EventType::MousePress>([&](const IEvent& evnt) {
            const auto& mousePressEvent =
            static_cast<const MousePressEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            m_MouseMap[mousePressEvent.Button()] = true;

            return true;
        });

        dispatcher.Dispatch<EventType::MouseRelease>([&](const IEvent& evnt) {
            const auto& mouseReleaseEvent =
            static_cast<const MouseReleaseEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            m_MouseMap[mouseReleaseEvent.Button()] = false;

            return true;
        });

        dispatcher.Dispatch<EventType::MouseMove>([&](const IEvent& evnt) {
            const auto& mouseMoveEvent = static_cast<const MouseMoveEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            m_MousePosition            = mouseMoveEvent.Position();
            m_RelativeMousePosition += mouseMoveEvent.RelativePosition();

            return true;
        });

        dispatcher.Dispatch<EventType::MouseWheel>([&](const IEvent& evnt) {
            const auto& mouseWheelEvent =
            static_cast<const MouseWheelEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            m_ScrollAmount += mouseWheelEvent.ScrollAmount();

            return true;
        });
    }

    void InputController::NewFrame()
    {
        m_RelativeMousePosition = Position2DI{ 0, 0 };
        m_ScrollAmount          = 0;
    }

    auto InputController::KeyPressed(KeyCode key) const -> bool
    {
        auto keyIt = m_KeyMap.find(key);
        if (keyIt != std::end(m_KeyMap)) { return keyIt->second; }

        return false;
    }

    auto InputController::MousePressed(MouseButton button) const -> bool
    {
        auto buttonIt = m_MouseMap.find(button);
        if (buttonIt != std::end(m_MouseMap)) { return buttonIt->second; }

        return false;
    }

}// namespace JE