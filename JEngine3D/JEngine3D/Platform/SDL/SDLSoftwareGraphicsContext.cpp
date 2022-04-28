#include "SDLSoftwareGraphicsContext.hpp"
#include "JEngine3D/Core/Base.hpp"

#include <SDL_pixels.h>
#include <cstring>// IWYU pragma: keep
#include <SDL_render.h>

namespace JE {

static constexpr auto CLEAR_COLOR = 0xFF111111;

SDLSoftwareGraphicsContext::SDLSoftwareGraphicsContext(IPlatformBackend::NativeWindowHandle windowHandle,
  IGraphicsContextCreator::NativeContextHandle contextHandle)
  : IGraphicsContext(windowHandle, contextHandle)
{
  auto size = RendererSize();
  m_Texture = static_cast<void *>(SDL_CreateTexture(static_cast<SDL_Renderer *>(NativeContextHandle()),
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STREAMING,
    size.Width,
    size.Height));
}

auto SDLSoftwareGraphicsContext::DrawableSize() -> Size2DI
{
  Size2DI size{};
  SDL_QueryTexture(static_cast<SDL_Texture *>(m_Texture), nullptr, nullptr, &size.Width, &size.Height);
  return size;
}

void SDLSoftwareGraphicsContext::Resize(const Size2DI &size)
{
  JE::UNUSED(size);
  ASSERT(m_PixelPtr == nullptr, "Can't resize current context");
  SDL_DestroyTexture(static_cast<SDL_Texture *>(m_Texture));

  auto rendererSize = RendererSize();
  m_Texture = static_cast<void *>(SDL_CreateTexture(static_cast<SDL_Renderer *>(NativeContextHandle()),
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STREAMING,
    rendererSize.Width,
    rendererSize.Height));
}

void SDLSoftwareGraphicsContext::MakeCurrent()
{
  if (m_PixelPtr == nullptr) { SDL_LockTexture(static_cast<SDL_Texture *>(m_Texture), nullptr, &m_PixelPtr, &m_Pitch); }
}
void SDLSoftwareGraphicsContext::SwapBuffers()
{
  m_PixelPtr = nullptr;
  SDL_UnlockTexture(static_cast<SDL_Texture *>(m_Texture));

  auto *renderer = static_cast<SDL_Renderer *>(NativeContextHandle());
  SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(m_Texture), nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void SDLSoftwareGraphicsContext::Destroy()
{
  auto *renderer = static_cast<SDL_Renderer *>(NativeContextHandle());
  SDL_DestroyRenderer(renderer);
}

void SDLSoftwareGraphicsContext::Clear()
{
  ASSERT(m_PixelPtr != nullptr, "Not current context");
  std::fill_n(static_cast<uint32_t *>(m_PixelPtr), DrawableSize().Width * DrawableSize().Height, CLEAR_COLOR);
}

auto SDLSoftwareGraphicsContext::RendererSize() -> Size2DI
{
  Size2DI size{};
  SDL_GetRendererOutputSize(static_cast<SDL_Renderer *>(NativeContextHandle()), &size.Width, &size.Height);
  return size;
}

}// namespace JE