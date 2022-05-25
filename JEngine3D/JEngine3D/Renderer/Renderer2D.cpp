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

  Data.TriangleVertices.reserve(MAX_TRIANGLES_PER_BATCH);
  Data.TriangleIndices.reserve(MAX_TRIANGLE_INDICES);
}

void Renderer2D::InitializeBatch(IDrawTarget *target)
{
  Data.BatchBegun = true;
  Data.Target = target;
}

void Renderer2D::Flush()
{
  ASSERT(Data.Target != nullptr, "IDrawTarget is missing");

  Data.Stats.FrameTriangleVertexCount += Data.TriangleVertices.size();
  Data.Stats.FrameTriangleIndexCount += Data.TriangleIndices.size();

  if (!Data.TriangleVertices.empty() && !Data.TriangleIndices.empty()) {
    Data.Target->Bind();
    Application::Get().RendererAPI().DrawVerticesIndexed(Data.TriangleVertices, Data.TriangleIndices);
    Data.Target->UnBind();
    Data.Stats.FrameDrawCalls++;
  }

  Data.BatchBegun = false;
  Data.Target = nullptr;
  Data.TriangleVertices.clear();
  Data.TriangleIndices.clear();
}

void Renderer2D::NextBatch()
{
  auto *target = Data.Target;
  Flush();
  InitializeBatch(target);
}

void Renderer2D::NewFrame()
{
  Data.Stats.FrameTriangleVertexCount = 0;
  Data.Stats.FrameTriangleIndexCount = 0;
  Data.Stats.FrameDrawCalls = 0;
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
  if (TriangleCount() + 1 > Data.TrianglesPerBatch) { NextBatch(); }

  auto vertexOffset = Data.TriangleVertices.size();

  Data.TriangleVertices.emplace_back(vertex0);
  Data.TriangleVertices.emplace_back(vertex1);
  Data.TriangleVertices.emplace_back(vertex2);

  auto indexOffset = Data.TriangleIndices.size();
  Data.TriangleIndices.resize(indexOffset + 3);// NOLINT
  Data.TriangleIndices[indexOffset] = static_cast<uint32_t>(vertexOffset);
  Data.TriangleIndices[indexOffset + 1] = static_cast<uint32_t>(vertexOffset + 1);
  Data.TriangleIndices[indexOffset + 2] = static_cast<uint32_t>(vertexOffset + 2);
}


void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Color &color)
{
  if (TriangleCount() + 2 > Data.TrianglesPerBatch) { NextBatch(); }

  auto vertexOffset = Data.TriangleVertices.size();

  Data.TriangleVertices.emplace_back(position, color);
  Data.TriangleVertices.emplace_back(glm::vec3{ position.x + size.x, position.y, position.z }, color);// NOLINT
  Data.TriangleVertices.emplace_back(glm::vec3{ position.x + size.x, position.y + size.y, position.z }, color);// NOLINT
  Data.TriangleVertices.emplace_back(glm::vec3{ position.x, position.y + size.y, position.z }, color);// NOLINT

  auto indexOffset = Data.TriangleIndices.size();
  Data.TriangleIndices.resize(indexOffset + 6);// NOLINT
  Data.TriangleIndices[indexOffset] = static_cast<uint32_t>(vertexOffset);
  Data.TriangleIndices[indexOffset + 1] = static_cast<uint32_t>(vertexOffset + 1);
  Data.TriangleIndices[indexOffset + 2] = static_cast<uint32_t>(vertexOffset + 2);
  Data.TriangleIndices[indexOffset + 3] = static_cast<uint32_t>(vertexOffset + 2);
  Data.TriangleIndices[indexOffset + 4] = static_cast<uint32_t>(vertexOffset + 3);
  Data.TriangleIndices[indexOffset + 5] = static_cast<uint32_t>(vertexOffset);// NOLINT
}

}// namespace JE