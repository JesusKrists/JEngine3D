set(UNITTEST_SOURCES
    UnitTests/Core/Application.cpp
    UnitTests/Core/Base.cpp
    UnitTests/Core/Events.cpp
    UnitTests/Core/InputController.cpp
    UnitTests/Core/LoggerController.cpp
    UnitTests/Core/MemoryController.cpp
    UnitTests/Core/Types.cpp
    UnitTests/Core/WindowController.cpp)

set(UNITTEST_PLATFORM_SOURCES UnitTests/Platform/IPlatformBackend.cpp)

set(CONSTEXPR_SOURCES UnitTests/Core/Base_constexpr.cpp UnitTests/Core/Types_constexpr.cpp)
