#pragma once

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"

namespace JE {

    class Renderer2DDebugView final : public IImGuiDebugView
    {
    public:
        Renderer2DDebugView() : IImGuiDebugView("Renderer2DDebugView") {}

    private:
        void OnImGuiRender() override;
    };

}// namespace JE