#pragma once

#include <JEngine3D/Renderer/IVertexArray.hpp>

class TestVertexArray final : public JE::IVertexArray
{
public:
    inline void Bind() const override {}
    inline void Unbind() const override {}

    inline void Reset() override {}
    inline void Delete() override {}

protected:
    inline void ConfigureVertexBufferLayout(const JE::BufferLayout& bufferLayout) override { JE::UNUSED(bufferLayout); }
};