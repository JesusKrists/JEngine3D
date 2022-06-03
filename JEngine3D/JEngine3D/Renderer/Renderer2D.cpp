#include "Renderer2D.hpp"


#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"
#include "JEngine3D/Core/WindowController.hpp"// for Window

#include <glm/gtx/transform.hpp>

namespace JE {

static Renderer2D *s_Renderer2DInstance = nullptr;// NOLINT

static constexpr std::array<glm::vec4, 4> QUAD_VERTEX_POSITION = { glm::vec4{ -0.5F, -0.5F, 0.0F, 1.0F },
  glm::vec4{ 0.5F, -0.5F, 0.0F, 1.0F },
  glm::vec4{ 0.5F, 0.5F, 0.0F, 1.0F },
  glm::vec4{ -0.5F, 0.5F, 0.0F, 1.0F } };

static constexpr std::array<glm::vec2, 4> QUAD_UV_COORDINATES = { glm::vec2{ 0, 0 },
  glm::vec2{ 1, 0 },
  glm::vec2{ 1, 1 },
  glm::vec2{ 0, 1 } };

Renderer2D::Renderer2D()
{
  ASSERT(s_Renderer2DInstance == nullptr, "Renderer2D instance already exists");
  s_Renderer2DInstance = this;

  Data.TriangleVertices.reserve(MAX_TRIANGLES_PER_BATCH);
  Data.TriangleIndices.reserve(MAX_TRIANGLE_INDICES);

  Data.VertexArray->AddVertexBuffer(*Data.VertexBuffer);
  Data.VertexArray->SetIndexBuffer(*Data.IndexBuffer);
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

  Data.VertexBuffer->SetData({ reinterpret_cast<const uint8_t *>(Data.TriangleVertices.data()),// NOLINT
    Data.TriangleVertices.size() * sizeof(Vertex) });

  Data.IndexBuffer->SetData(Data.TriangleIndices);

  if (!Data.TriangleVertices.empty() && !Data.TriangleIndices.empty()) {

    if (Data.TextureSlotIndex != -1) {
      for (uint32_t i = 0; i <= static_cast<uint32_t>(Data.TextureSlotIndex); i++) {
        Data.TextureSlots[i]->Bind(i);// NOLINT
      }
    }

    Data.Target->Bind();
    JE_APP.RendererAPI().DrawIndexed(*Data.VertexArray);
    Data.Target->Unbind();
    Data.Stats.FrameDrawCalls++;
  }

  Data.BatchBegun = false;
  Data.Target = nullptr;
  Data.TriangleVertices.clear();
  Data.TriangleIndices.clear();
  Data.TextureSlotIndex = -1;
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
    InitializeBatch(&JE_APP.MainWindow().GraphicsContext());
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
  Data.TriangleIndices.resize(indexOffset + 3);
  Data.TriangleIndices[indexOffset] = static_cast<uint32_t>(vertexOffset);
  Data.TriangleIndices[indexOffset + 1] = static_cast<uint32_t>(vertexOffset + 1);
  Data.TriangleIndices[indexOffset + 2] = static_cast<uint32_t>(vertexOffset + 2);
}

void Renderer2D::DrawTriangle(Vertex &vertex0, Vertex &vertex1, Vertex &vertex2, const ITexture &texture)
{
  if (TriangleCount() + 1 > Data.TrianglesPerBatch) { NextBatch(); }

  if (Data.TextureSlotIndex == -1) {
    Data.TextureSlots[static_cast<uint32_t>(++Data.TextureSlotIndex)] = &texture;// NOLINT
  } else {
    if (static_cast<uint32_t>(Data.TextureSlotIndex) >= MAX_TEXTURE_SLOTS) { NextBatch(); }
    Data.TextureSlots[static_cast<uint32_t>(++Data.TextureSlotIndex)] = &texture;// NOLINT
  }

  vertex0.TextureIndex = Data.TextureSlotIndex;
  vertex1.TextureIndex = Data.TextureSlotIndex;
  vertex2.TextureIndex = Data.TextureSlotIndex;

  DrawTriangle(vertex0, vertex1, vertex2);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Color &color)
{
  glm::mat4 transform =
    glm::translate(glm::mat4(1.0F), position) * glm::scale(glm::mat4(1.0F), { size.x, size.y, 1.0F });// NOLINT
  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position,
  const glm::vec2 &size,
  const ITexture &texture,
  const Color &tintColor)
{
  glm::mat4 transform =
    glm::translate(glm::mat4(1.0F), position) * glm::scale(glm::mat4(1.0F), { size.x, size.y, 1.0F });// NOLINT
  DrawQuad(transform, texture, tintColor);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Color &color)
{
  if (TriangleCount() + 2 > Data.TrianglesPerBatch) { NextBatch(); }

  auto vertexOffset = Data.TriangleVertices.size();

  Data.TriangleVertices.emplace_back(transform * QUAD_VERTEX_POSITION[0], color);
  Data.TriangleVertices.emplace_back(transform * QUAD_VERTEX_POSITION[1], color);
  Data.TriangleVertices.emplace_back(transform * QUAD_VERTEX_POSITION[2], color);
  Data.TriangleVertices.emplace_back(transform * QUAD_VERTEX_POSITION[3], color);

  auto indexOffset = Data.TriangleIndices.size();
  Data.TriangleIndices.resize(indexOffset + 6);// NOLINT
  Data.TriangleIndices[indexOffset] = static_cast<uint32_t>(vertexOffset);
  Data.TriangleIndices[indexOffset + 1] = static_cast<uint32_t>(vertexOffset + 1);
  Data.TriangleIndices[indexOffset + 2] = static_cast<uint32_t>(vertexOffset + 2);
  Data.TriangleIndices[indexOffset + 3] = static_cast<uint32_t>(vertexOffset + 2);
  Data.TriangleIndices[indexOffset + 4] = static_cast<uint32_t>(vertexOffset + 3);
  Data.TriangleIndices[indexOffset + 5] = static_cast<uint32_t>(vertexOffset);// NOLINT
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const ITexture &texture, const Color &tintColor)
{
  if (TriangleCount() + 2 > Data.TrianglesPerBatch) { NextBatch(); }

  if (Data.TextureSlotIndex == -1) {
    Data.TextureSlots[static_cast<uint32_t>(++Data.TextureSlotIndex)] = &texture;// NOLINT
  } else {
    if (static_cast<uint32_t>(Data.TextureSlotIndex) >= MAX_TEXTURE_SLOTS) { NextBatch(); }
    Data.TextureSlots[static_cast<uint32_t>(++Data.TextureSlotIndex)] = &texture;// NOLINT
  }

  auto vertexOffset = Data.TriangleVertices.size();

  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[0], tintColor, QUAD_UV_COORDINATES[0], Data.TextureSlotIndex);
  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[1], tintColor, QUAD_UV_COORDINATES[1], Data.TextureSlotIndex);
  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[2], tintColor, QUAD_UV_COORDINATES[2], Data.TextureSlotIndex);
  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[3], tintColor, QUAD_UV_COORDINATES[3], Data.TextureSlotIndex);

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