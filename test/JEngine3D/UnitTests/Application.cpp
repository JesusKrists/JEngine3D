/*#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>// for UNUSED

class ApplicationTestsFixture
{
public:
  ApplicationTestsFixture() { JE::UNUSED(backend.Initialize()); }

protected:
  TestPlatformBackend backend;
};

TEST_CASE_METHOD(ApplicationTestsFixture, "JE::Application creates MainWindow", "[JE::Application]") {}*/