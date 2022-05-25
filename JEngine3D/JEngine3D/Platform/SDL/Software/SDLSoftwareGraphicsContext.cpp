#include "SDLSoftwareGraphicsContext.hpp"
#include "JEngine3D/Core/Base.hpp"

#include "JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp"

#include <SDL_pixels.h>
#include <cstring>// IWYU pragma: keep
#include <SDL_render.h>

namespace JE {

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
  SDL_DestroyTexture(static_cast<SDL_Texture *>(m_Texture));

  auto rendererSize = RendererSize();
  m_Texture = static_cast<void *>(SDL_CreateTexture(static_cast<SDL_Renderer *>(NativeContextHandle()),
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STREAMING,
    rendererSize.Width,
    rendererSize.Height));
  if (m_PixelPtr != nullptr) {
    int pitch = 0;
    SDL_LockTexture(static_cast<SDL_Texture *>(m_Texture), nullptr, &m_PixelPtr, &pitch);
  }
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
  m_PixelPtr = nullptr;
  SDL_UnlockTexture(static_cast<SDL_Texture *>(m_Texture));
  SDL_DestroyTexture(static_cast<SDL_Texture *>(m_Texture));
  auto *renderer = static_cast<SDL_Renderer *>(NativeContextHandle());
  SDL_DestroyRenderer(renderer);
}

auto SDLSoftwareGraphicsContext::RendererSize() -> Size2DI
{
  Size2DI size{};
  SDL_GetRendererOutputSize(static_cast<SDL_Renderer *>(NativeContextHandle()), &size.Width, &size.Height);
  return size;
}

void SDLSoftwareGraphicsContext::MakeContextCurrent()
{
  if (m_PixelPtr == nullptr) {
    int pitch = 0;
    SDL_LockTexture(static_cast<SDL_Texture *>(m_Texture), nullptr, &m_PixelPtr, &pitch);
  }

  SoftwareRendererAPI::BindGraphicsContext(this);
}

}// namespace JE