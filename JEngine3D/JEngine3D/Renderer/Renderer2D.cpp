#include "Renderer2D.hpp"


#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"
#include "JEngine3D/Core/WindowController.hpp"// for Window

namespace JE {

static Renderer2D *s_Renderer2DInstance = nullptr;// NOLINT

Renderer2D::Renderer2D()
{
  ASSERT(s_Renderer2DInstance == nullptr, "Renderer2D instance already exists");
  s_Renderer2DInstance = this;

  Data.QuadVertices.reserve(MAX_QUAD_VERTICES);
  Data.QuadIndices.resize(MAX_QUAD_INDICES);

  Data.TriangleVertices.reserve(MAX_TRIANGLES_PER_BATCH);

  uint32_t offset = 0;
  for (uint32_t i = 0; i < MAX_QUAD_INDICES; i += 6) {// NOLINT
    Data.QuadIndices[i + 0] = offset + 0;
    Data.QuadIndices[i + 1] = offset + 1;
    Data.QuadIndices[i + 2] = offset + 2;

    Data.QuadIndices[i + 3] = offset + 2;
    Data.QuadIndices[i + 4] = offset + 3;
    Data.QuadIndices[i + 5] = offset + 0;// NOLINT

    offset += 4;
  }
}

void Renderer2D::InitializeBatch(IDrawTarget *target)
{
  Data.BatchBegun = true;
  Data.Target = target;
}

void Renderer2D::Flush()
{
  Data.Stats.FrameQuadVertexCount += QuadVertexCount();
  Data.Stats.FrameTriangleVertexCount += TriangleVertexCount();

  ASSERT(Data.Target != nullptr, "IDrawTarget is missing");

  if (!Data.QuadVertices.empty()) { Data.Target->DrawVerticesIndexed(Data.QuadVertices, Data.QuadIndices); }
  if (!Data.TriangleVertices.empty()) { Data.Target->DrawVertices(Data.TriangleVertices); }

  Data.BatchBegun = false;
  Data.Target = nullptr;
  Data.QuadVertices.clear();
  Data.TriangleVertices.clear();
}

void Renderer2D::NextBatch()
{
  auto *target = Data.Target;
  Flush();
  InitializeBatch(target);
}

void Renderer2D::NewFrame()
{
  Data.Stats.FrameQuadVertexCount = 0;
  Data.Stats.FrameTriangleVertexCount = 0;
}

void Renderer2D::BeginBatch(IDrawTarget *target)
{
  if (Data.BatchBegun) { Flush(); }
  if (target != nullptr) {
    InitializeBatch(target);
  } else {
    InitializeBatch(&Application::Get().MainWindow().GraphicsContext());
  }
}

void Renderer2D::EndBatch()
{
  if (!Data.BatchBegun) { return; }
  Flush();
}

void Renderer2D::DrawTriangle(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2)
{
  Data.TriangleVertices.push_back(vertex0);
  Data.TriangleVertices.push_back(vertex1);
  Data.TriangleVertices.push_back(vertex2);
}

}// namespace JE