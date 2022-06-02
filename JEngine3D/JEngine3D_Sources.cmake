set(JEngine3D_HEADERS
    JEngine3D/Core/ImGui/ImGuiLayer.hpp
    JEngine3D/Core/ImGui/ImGuiSoftwareRenderer.hpp
    JEngine3D/Core/ImGui/ImGuiSupport.hpp
    JEngine3D/Core/Application.hpp
    JEngine3D/Core/Assert.hpp
    JEngine3D/Core/Base.hpp
    JEngine3D/Core/Events.hpp
    JEngine3D/Core/ILayer.hpp
    JEngine3D/Core/InputController.hpp
    JEngine3D/Core/KeyCodes.hpp
    JEngine3D/Core/LayerStack.hpp
    JEngine3D/Core/LoggerController.hpp
    JEngine3D/Core/MemoryController.hpp
    JEngine3D/Core/MouseButtons.hpp
    JEngine3D/Core/Types.hpp
    JEngine3D/Core/WindowController.hpp
    JEngine3D/Debug/View/ApplicationDebugView.hpp
    JEngine3D/Debug/View/IImGuiDebugView.hpp
    JEngine3D/Debug/View/InputControllerDebugView.hpp
    JEngine3D/Debug/View/MemoryControllerDebugView.hpp
    JEngine3D/Debug/View/Renderer2DDebugView.hpp
    JEngine3D/Debug/View/WindowControllerDebugView.hpp
    JEngine3D/Platform/IGraphicsContext.hpp
    JEngine3D/Platform/IGraphicsContextCreator.hpp
    JEngine3D/Platform/IPlatformBackend.hpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContext.hpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp
    JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContext.hpp
    JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContextCreator.hpp
    JEngine3D/Platform/SDL/SDLPlatformBackend.hpp
    JEngine3D/Renderer/ImGui/ImGuiRenderer.hpp
    JEngine3D/Renderer/Software/ISoftwareShader.hpp
    JEngine3D/Renderer/Software/SoftwareBuffer.hpp
    JEngine3D/Renderer/Software/SoftwareFrameBufferObject.hpp
    JEngine3D/Renderer/Software/SoftwareRasterizer.hpp
    JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp
    JEngine3D/Renderer/Software/SoftwareRendererObjectCreator.hpp
    JEngine3D/Renderer/Software/SoftwareTexture.hpp
    JEngine3D/Renderer/IBuffer.hpp
    JEngine3D/Renderer/IDrawTarget.hpp
    JEngine3D/Renderer/IRendererAPI.hpp
    JEngine3D/Renderer/IRendererObjectCreator.hpp
    JEngine3D/Renderer/IShader.hpp
    JEngine3D/Renderer/ITexture.hpp
    JEngine3D/Renderer/IVertexArray.hpp
    JEngine3D/Renderer/Renderer2D.hpp)

set(JEngine3D_SOURCES
    JEngine3D/Core/ImGui/ImGuiLayer.cpp
    JEngine3D/Core/ImGui/ImGuiSoftwareRenderer.cpp
    JEngine3D/Core/Application.cpp
    JEngine3D/Core/InputController.cpp
    JEngine3D/Core/LayerStack.cpp
    JEngine3D/Core/LoggerController.cpp
    JEngine3D/Core/MemoryController.cpp
    JEngine3D/Core/WindowController.cpp
    JEngine3D/Debug/View/ApplicationDebugView.cpp
    JEngine3D/Debug/View/InputControllerDebugView.cpp
    JEngine3D/Debug/View/MemoryControllerDebugView.cpp
    JEngine3D/Debug/View/Renderer2DDebugView.cpp
    JEngine3D/Debug/View/WindowControllerDebugView.cpp
    JEngine3D/Platform/IGraphicsContext.cpp
    JEngine3D/Platform/IGraphicsContextCreator.cpp
    JEngine3D/Platform/IPlatformBackend.cpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContext.cpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.cpp
    JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContext.cpp
    JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContextCreator.cpp
    JEngine3D/Platform/SDL/SDLPlatformBackend.cpp
    JEngine3D/Renderer/ImGui/ImGuiRenderer.cpp
    JEngine3D/Renderer/Software/SoftwareFrameBufferObject.cpp
    JEngine3D/Renderer/Software/SoftwareRasterizer.cpp
    JEngine3D/Renderer/Software/SoftwareRendererAPI.cpp
    JEngine3D/Renderer/Software/SoftwareTexture.cpp
    JEngine3D/Renderer/IRendererObjectCreator.cpp
    JEngine3D/Renderer/Renderer2D.cpp)
