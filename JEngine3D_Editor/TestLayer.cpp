#include "TestLayer.hpp"

#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/LoggerController.hpp>// for Logger

#include <exception>// for exception

#include <imgui.h>

namespace JE {
class IEvent;
}

static constexpr auto MILISECONDS = 1000;

namespace JEditor {

void TestLayer::OnCreate() {}
void TestLayer::OnDestroy() {}

void TestLayer::OnUpdate()
{
  JE::Logger::ClientLogger().info("App delta time - {}", JE::Application::Get().DeltaTime() * MILISECONDS);
}
void TestLayer::OnImGuiRender() { ImGui::ShowDemoWindow(); }

void TestLayer::OnEvent(JE::IEvent &event) { JE::UNUSED(event); }

}// namespace JEditor