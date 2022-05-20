#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"

namespace JE {

class IDrawTarget;

class Renderer2D
{
  friend class Application;

public:
  static constexpr size_t MAX_TRIANGLES_PER_BATCH = 50000;
  static constexpr size_t MAX_TRIANGLE_INDICES = MAX_TRIANGLES_PER_BATCH * 3;

  void NewFrame();

  void BeginBatch(IDrawTarget *target = nullptr);
  void EndBatch();

  void DrawTriangle(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2);
  void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Color &color);

  [[nodiscard]] inline auto TrianglesPerBatch() const -> size_t { return Data.TrianglesPerBatch; }
  inline void SetTrianglesPerBatch(size_t numTriangles)
  {
    ASSERT(numTriangles != 0, "Cannot have 0 triangles in a batch");
    Data.TrianglesPerBatch = std::min(numTriangles, MAX_TRIANGLES_PER_BATCH);
  }

  [[nodiscard]] inline auto FrameTriangleVertexCount() const -> size_t { return Data.Stats.FrameTriangleVertexCount; }
  [[nodiscard]] inline auto FrameTriangleIndexCount() const -> size_t { return Data.Stats.FrameTriangleIndexCount; }
  [[nodiscard]] inline auto FrameTriangleCount() const -> size_t { return Data.Stats.FrameTriangleIndexCount / 3; }
  [[nodiscard]] inline auto FrameDrawCalls() const -> size_t { return Data.Stats.FrameDrawCalls; }

private:
  Renderer2D();

  [[nodiscard]] inline auto TriangleCount() const -> size_t { return Data.TriangleIndices.size() / 3; }

  void InitializeBatch(IDrawTarget *target);
  void Flush();
  void NextBatch();

  struct Renderer2DData
  {
    struct Stats
    {
      size_t FrameTriangleVertexCount = 0;
      size_t FrameTriangleIndexCount = 0;
      size_t FrameDrawCalls = 0;
    } Stats;

    size_t TrianglesPerBatch = MAX_TRIANGLES_PER_BATCH;

    bool BatchBegun = false;
    IDrawTarget *Target = nullptr;

    Vector<Vertex, MemoryTag::Renderer> TriangleVertices;
    Vector<uint32_t, MemoryTag::Renderer> TriangleIndices;
  };

  Renderer2DData Data;
};

}// namespace JE