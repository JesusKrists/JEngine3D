#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/WindowController.hpp"

#include "JEngine3D/Platform/IGraphicsContext.hpp"

namespace JE {

class IDrawTarget;

class Renderer2D
{
  friend class Application;

public:
  static constexpr size_t INITIAL_QUADS_PER_BATCH = 1000;
  static constexpr size_t MAX_QUADS_PER_BATCH = 20000;
  static constexpr size_t MAX_QUAD_VERTICES = MAX_QUADS_PER_BATCH * 4;
  static constexpr size_t MAX_QUAD_INDICES = MAX_QUADS_PER_BATCH * 6;

  static constexpr size_t MAX_TRIANGLES_PER_BATCH = 50000;

  void NewFrame();

  void BeginBatch(IDrawTarget *target = nullptr);
  void EndBatch();

  void DrawTriangle(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2);

  [[nodiscard]] inline auto QuadsPerBatch() const -> size_t { return Data.QuadsPerBatch; }
  inline void SetQuadsPerBatch(size_t numQuads)
  {
    ASSERT(numQuads != 0, "Cannot have 0 quads in a batch");
    Data.QuadsPerBatch = std::min(numQuads, MAX_QUADS_PER_BATCH);
  }

  [[nodiscard]] inline auto TrianglesPerBatch() const -> size_t { return Data.TrianglesPerBatch; }
  inline void SetTrianglesPerBatch(size_t numTriangles)
  {
    ASSERT(numTriangles != 0, "Cannot have 0 triangles in a batch");
    Data.TrianglesPerBatch = std::min(numTriangles, MAX_TRIANGLES_PER_BATCH);
  }

  [[nodiscard]] inline auto QuadVertexCount() const -> size_t { return Data.QuadVertices.size(); }
  [[nodiscard]] inline auto TriangleVertexCount() const -> size_t { return Data.TriangleVertices.size(); }

  [[nodiscard]] inline auto FrameQuadVertexCount() const -> size_t { return Data.Stats.FrameQuadVertexCount; }
  [[nodiscard]] inline auto FrameTriangleVertexCount() const -> size_t { return Data.Stats.FrameTriangleVertexCount; }

private:
  Renderer2D();

  void InitializeBatch(IDrawTarget *target);
  void Flush();
  void NextBatch();

  struct Renderer2DData
  {
    struct Stats
    {
      size_t FrameQuadVertexCount = 0;
      size_t FrameTriangleVertexCount = 0;
    } Stats;

    size_t QuadsPerBatch = INITIAL_QUADS_PER_BATCH;
    size_t TrianglesPerBatch = MAX_TRIANGLES_PER_BATCH;

    bool BatchBegun = false;
    IDrawTarget *Target = nullptr;

    Vector<Vertex, MemoryTag::Renderer> QuadVertices;
    Vector<uint32_t, MemoryTag::Renderer> QuadIndices;

    Vector<Vertex, MemoryTag::Renderer> TriangleVertices;
  };

  Renderer2DData Data;
};

}// namespace JE