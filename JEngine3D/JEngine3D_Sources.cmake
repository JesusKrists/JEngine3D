set(JEngine3D_HEADERS
    JEngine3D/Core/ImGui/config.hpp
    JEngine3D/Core/ImGui/ImGuiLayer.hpp
    JEngine3D/Core/ImGui/ImGuiSupport.hpp
    JEngine3D/Core/Application.hpp
    JEngine3D/Core/Assert.hpp
    JEngine3D/Core/AssetController.hpp
    JEngine3D/Core/Base.hpp
    JEngine3D/Core/Events.hpp
    JEngine3D/Core/ILayer.hpp
    JEngine3D/Core/InputController.hpp
    JEngine3D/Core/KeyCodes.hpp
    JEngine3D/Core/LayerStack.hpp
    JEngine3D/Core/LoggerController.hpp
    JEngine3D/Core/MemoryController.hpp
    JEngine3D/Core/MouseButtons.hpp
    JEngine3D/Core/PeriodicTimer.hpp
    JEngine3D/Core/PluginController.hpp
    JEngine3D/Core/Types.hpp
    JEngine3D/Core/WindowController.hpp
    JEngine3D/Debug/View/ApplicationDebugView.hpp
    JEngine3D/Debug/View/IImGuiDebugView.hpp
    JEngine3D/Debug/View/InputControllerDebugView.hpp
    JEngine3D/Debug/View/MemoryControllerDebugView.hpp
    JEngine3D/Debug/View/Renderer2DDebugView.hpp
    JEngine3D/Debug/View/WindowControllerDebugView.hpp
    JEngine3D/Debug/NewOverrideDebug.hpp
    JEngine3D/ECS/Components.hpp
    JEngine3D/ECS/Entity.hpp
    JEngine3D/Platform/IGraphicsContext.hpp
    JEngine3D/Platform/IGraphicsContextCreator.hpp
    JEngine3D/Platform/IPlatform.hpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContext.hpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp
    JEngine3D/Platform/SDL/SDLPlatform.hpp
    JEngine3D/Renderer/ImGui/ImGuiRenderer.hpp
    JEngine3D/Renderer/OpenGL/OpenGLBuffer.hpp
    JEngine3D/Renderer/OpenGL/OpenGLFramebuffer.hpp
    JEngine3D/Renderer/OpenGL/OpenGLRendererAPI.hpp
    JEngine3D/Renderer/OpenGL/OpenGLRendererObjectCreator.hpp
    JEngine3D/Renderer/OpenGL/OpenGLShader.hpp
    JEngine3D/Renderer/OpenGL/OpenGLTexture.hpp
    JEngine3D/Renderer/OpenGL/OpenGLVertexArray.hpp
    JEngine3D/Renderer/IBuffer.hpp
    JEngine3D/Renderer/IDrawTarget.hpp
    JEngine3D/Renderer/IFramebuffer.hpp
    JEngine3D/Renderer/IRendererAPI.hpp
    JEngine3D/Renderer/IRendererObjectCreator.hpp
    JEngine3D/Renderer/IShader.hpp
    JEngine3D/Renderer/ITexture.hpp
    JEngine3D/Renderer/IVertexArray.hpp
    JEngine3D/Renderer/Renderer2D.hpp
    JEngine3D/Utility/ImageLoader.hpp
    JEngine3D/Utility/Math.hpp)

set(JEngine3D_SOURCES
    JEngine3D/Core/ImGui/ImGuiLayer.cpp
    JEngine3D/Core/Application.cpp
    JEngine3D/Core/AssetController.cpp
    JEngine3D/Core/InputController.cpp
    JEngine3D/Core/LayerStack.cpp
    JEngine3D/Core/LoggerController.cpp
    JEngine3D/Core/MemoryController.cpp
    JEngine3D/Core/PluginController.cpp
    JEngine3D/Core/WindowController.cpp
    JEngine3D/Debug/View/ApplicationDebugView.cpp
    JEngine3D/Debug/View/InputControllerDebugView.cpp
    JEngine3D/Debug/View/MemoryControllerDebugView.cpp
    JEngine3D/Debug/View/Renderer2DDebugView.cpp
    JEngine3D/Debug/View/WindowControllerDebugView.cpp
    JEngine3D/Debug/NewOverrideDebug.cpp
    JEngine3D/Platform/IGraphicsContext.cpp
    JEngine3D/Platform/IGraphicsContextCreator.cpp
    JEngine3D/Platform/IPlatform.cpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContext.cpp
    JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.cpp
    JEngine3D/Platform/SDL/SDLPlatform.cpp
    JEngine3D/Renderer/ImGui/ImGuiRenderer.cpp
    JEngine3D/Renderer/OpenGL/OpenGLBuffer.cpp
    JEngine3D/Renderer/OpenGL/OpenGLFramebuffer.cpp
    JEngine3D/Renderer/OpenGL/OpenGLRendererAPI.cpp
    JEngine3D/Renderer/OpenGL/OpenGLRendererObjectCreator.cpp
    JEngine3D/Renderer/OpenGL/OpenGLShader.cpp
    JEngine3D/Renderer/OpenGL/OpenGLTexture.cpp
    JEngine3D/Renderer/OpenGL/OpenGLVertexArray.cpp
    JEngine3D/Renderer/IRendererObjectCreator.cpp
    JEngine3D/Renderer/Renderer2D.cpp
    JEngine3D/Utility/ImageLoader.cpp)
