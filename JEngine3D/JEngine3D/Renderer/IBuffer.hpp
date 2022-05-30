#pragma once

#include "JEngine3D/Core/Types.hpp"

#include <span>

namespace JE {


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IVertexBuffer
{
public:
  virtual ~IVertexBuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void SetData(const std::span<const uint8_t> &data) = 0;

  // TODO(JesusKrists): LAYOUT STUFF
};


}// namespace JE