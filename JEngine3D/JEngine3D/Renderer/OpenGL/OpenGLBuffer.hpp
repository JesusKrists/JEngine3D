#pragma once

#include "JEngine3D/Renderer/IBuffer.hpp"

#include <cstdint>// for uint32_t
#include <cstddef>// for size_t, byte
#include <span>// for span

namespace JE {

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class OpenGLVertexBuffer final : public IVertexBuffer
    {
    public:
        explicit OpenGLVertexBuffer(const JE::BufferLayout& layout);
        virtual ~OpenGLVertexBuffer();// NOLINT

        void Bind() const override;
        void Unbind() const override;

        void SetData(const std::span<const std::byte>& data) override;

    private:
        uint32_t m_RendererID      = 0;
        size_t   m_TotalBufferSize = 0;
    };

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class OpenGLIndexBuffer final : public IIndexBuffer
    {
    public:
        OpenGLIndexBuffer();
        explicit OpenGLIndexBuffer(const std::span<const uint32_t>& data);
        virtual ~OpenGLIndexBuffer();// NOLINT

        void Bind() const override;
        void Unbind() const override;

    private:
        void UploadData(const std::span<const uint32_t>& data) override;

        uint32_t m_RendererID      = 0;
        size_t   m_TotalBufferSize = 0;
    };

}// namespace JE