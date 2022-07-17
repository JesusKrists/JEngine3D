#include "Renderer2D.hpp"


#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"
#include "JEngine3D/Core/WindowController.hpp"// for Window

#include <JEngine3D/Renderer/IRendererObjectCreator.hpp>
#include <JEngine3D/Renderer/IShader.hpp>
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

static constexpr std::string_view VERTEX_SHADER_SOURCE =
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_Position;\n"
  "layout (location = 1) in vec4 a_Color;\n"
  "layout (location = 2) in vec2 a_UV;\n"
  "layout (location = 3) in int a_TexIndex;\n"
  "out vec4 Frag_Color;\n"
  "out vec2 Frag_UV;\n"
  "flat out int Frag_TexIndex;\n"
  "void main()\n"
  "{\n"
  "    Frag_Color = a_Color;\n"
  "    Frag_UV = a_UV;\n"
  "    Frag_TexIndex = a_TexIndex;\n"
  "    gl_Position = vec4(a_Position.xyz, 1);\n"
  "}\n";

static constexpr std::string_view FRAGMENT_SHADER_SOURCE =
  "#version 330 core\n"
  "layout (location = 0) out vec4 Out_Color;\n"
  "in vec4 Frag_Color;\n"
  "in vec2 Frag_UV;\n"
  "flat in int Frag_TexIndex;\n"
  "uniform sampler2D u_Texture[16];\n"
  "void main()\n"
  "{\n"
  "    if (Frag_TexIndex != -1) {\n"
  "        switch (Frag_TexIndex) {\n"
  "            case  0: Out_Color = Frag_Color * texture(u_Texture[0], Frag_UV); break;\n"
  "            case  1: Out_Color = Frag_Color * texture(u_Texture[1], Frag_UV); break;\n"
  "            case  2: Out_Color = Frag_Color * texture(u_Texture[2], Frag_UV); break;\n"
  "            case  3: Out_Color = Frag_Color * texture(u_Texture[3], Frag_UV); break;\n"
  "            case  4: Out_Color = Frag_Color * texture(u_Texture[4], Frag_UV); break;\n"
  "            case  5: Out_Color = Frag_Color * texture(u_Texture[5], Frag_UV); break;\n"
  "            case  6: Out_Color = Frag_Color * texture(u_Texture[6], Frag_UV); break;\n"
  "            case  7: Out_Color = Frag_Color * texture(u_Texture[7], Frag_UV); break;\n"
  "            case  8: Out_Color = Frag_Color * texture(u_Texture[8], Frag_UV); break;\n"
  "            case  9: Out_Color = Frag_Color * texture(u_Texture[9], Frag_UV); break;\n"
  "            case 10: Out_Color = Frag_Color * texture(u_Texture[10], Frag_UV); break;\n"
  "            case 11: Out_Color = Frag_Color * texture(u_Texture[11], Frag_UV); break;\n"
  "            case 12: Out_Color = Frag_Color * texture(u_Texture[12], Frag_UV); break;\n"
  "            case 13: Out_Color = Frag_Color * texture(u_Texture[13], Frag_UV); break;\n"
  "            case 14: Out_Color = Frag_Color * texture(u_Texture[14], Frag_UV); break;\n"
  "            case 15: Out_Color = Frag_Color * texture(u_Texture[15], Frag_UV); break;\n"
  "        }\n"
  "    } else {\n"
  "        Out_Color = Frag_Color;\n"
  "    }\n"
  "}\n";

Renderer2D::Renderer2D()
{
  ZoneScopedN("Renderer2D::Renderer2D");// NOLINT
  ASSERT(s_Renderer2DInstance == nullptr, "Renderer2D instance already exists");
  s_Renderer2DInstance = this;

  Data.TriangleVertices.reserve(MAX_TRIANGLES_PER_BATCH);
  Data.TriangleIndices.reserve(MAX_TRIANGLE_INDICES);

  Data.VertexArray->AddVertexBuffer(*Data.VertexBuffer);
  Data.VertexArray->SetIndexBuffer(*Data.IndexBuffer);

  Data.Shader =
    IRendererObjectCreator::Get().CreateShader("Renderer2D Shader", VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
}

void Renderer2D::InitializeBatch(IDrawTarget *target)
{
  Data.BatchBegun = true;
  Data.Target = target;
}

void Renderer2D::Flush()
{
  ZoneScopedN("Renderer2D::Flush");// NOLINT
  ASSERT(Data.Target != nullptr, "IDrawTarget is missing");
  ASSERT(!Data.TriangleIndices.empty(), "Nothing will been drawn");

  Data.Stats.FrameTriangleVertexCount += Data.TriangleVertices.size();
  Data.Stats.FrameTriangleIndexCount += Data.TriangleIndices.size();

  Data.VertexBuffer->SetData({ reinterpret_cast<const std::byte *>(Data.TriangleVertices.data()),// NOLINT
    Data.TriangleVertices.size() * sizeof(Vertex) });

  Data.IndexBuffer->SetData(Data.TriangleIndices);

  if (!Data.TriangleVertices.empty() && !Data.TriangleIndices.empty()) {
    Data.Shader->Bind();

    if (Data.TextureSlotIndex != -1) {
      for (uint32_t i = 0; i <= static_cast<uint32_t>(Data.TextureSlotIndex); i++) {
        Data.TextureSlots[i]->Bind(static_cast<uint8_t>(i));// NOLINT
        Data.Shader->SetInt(TextureSamplerUniformString(static_cast<uint8_t>(i)), static_cast<int32_t>(i));
      }
    }

    Data.Target->Bind();

    JE_APP.RendererAPI().DrawIndexed(*Data.VertexArray);

    Data.Shader->Unbind();
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
  ZoneScopedN("Renderer2D::NextBatch");// NOLINT
  auto *target = Data.Target;
  Flush();
  InitializeBatch(target);
}

auto Renderer2D::PushTexture(const ITexture2D &texture) -> int8_t
{
  ZoneScopedN("Renderer2D::PushTexture");// NOLINT
  int8_t textureIndex = -1;
  if (Data.TextureSlotIndex == -1) {
    Data.TextureSlots[static_cast<size_t>(++Data.TextureSlotIndex)] = &texture;// NOLINT
    return Data.TextureSlotIndex;
  } else {

    for (int8_t i = 0; i <= Data.TextureSlotIndex; i++) {
      if (i == MAX_TEXTURE_SLOTS) { break; }

      if (Data.TextureSlots[static_cast<size_t>(i)] == &texture) {// NOLINT
        textureIndex = i;
        break;
      }
    }
  }

  if (textureIndex == -1) {
    if (static_cast<uint32_t>(Data.TextureSlotIndex) >= MAX_TEXTURE_SLOTS) { NextBatch(); }
    Data.TextureSlots[static_cast<size_t>(++Data.TextureSlotIndex)] = &texture;// NOLINT
    textureIndex = Data.TextureSlotIndex;
  }

  return textureIndex;
}

void Renderer2D::NewFrame()
{
  Data.Stats.FrameTriangleVertexCount = 0;
  Data.Stats.FrameTriangleIndexCount = 0;
  Data.Stats.FrameDrawCalls = 0;
}

void Renderer2D::BeginBatch(IDrawTarget *target)
{
  ZoneScopedN("Renderer2D::BeginBatch");// NOLINT
  if (Data.BatchBegun) { Flush(); }
  if (target != nullptr) {
    InitializeBatch(target);
  } else {
    InitializeBatch(&JE_APP.MainWindow().GraphicsContext());
  }
}

void Renderer2D::EndBatch()
{
  ZoneScopedN("Renderer2D::EndBatch");// NOLINT
  if (!Data.BatchBegun) { return; }
  Flush();
}

void Renderer2D::DrawTriangle(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2)
{
  ZoneScopedN("Renderer2D::DrawTriangle");// NOLINT
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

void Renderer2D::DrawTriangle(Vertex &vertex0, Vertex &vertex1, Vertex &vertex2, const ITexture2D &texture)
{
  ZoneScopedN("Renderer2D::DrawTriangle(texture)");// NOLINT
  if (TriangleCount() + 1 > Data.TrianglesPerBatch) { NextBatch(); }

  auto textureIndex = PushTexture(texture);

  vertex0.TextureIndex = textureIndex;// NOLINT
  vertex1.TextureIndex = textureIndex;// NOLINT
  vertex2.TextureIndex = textureIndex;// NOLINT

  DrawTriangle(vertex0, vertex1, vertex2);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Color &color)
{
  ZoneScopedN("Renderer2D::DrawQuad(pos, size)");// NOLINT
  glm::mat4 transform =
    glm::translate(glm::mat4(1.0F), position) * glm::scale(glm::mat4(1.0F), { size.x, size.y, 1.0F });// NOLINT
  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position,
  const glm::vec2 &size,
  const ITexture2D &texture,
  const Color &tintColor)
{
  ZoneScopedN("Renderer2D::DrawQuad(pos, size, texture)");// NOLINT
  glm::mat4 transform =
    glm::translate(glm::mat4(1.0F), position) * glm::scale(glm::mat4(1.0F), { size.x, size.y, 1.0F });// NOLINT
  DrawQuad(transform, texture, tintColor);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Color &color)
{
  ZoneScopedN("Renderer2D::DrawQuad(transform)");// NOLINT
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

void Renderer2D::DrawQuad(const glm::mat4 &transform, const ITexture2D &texture, const Color &tintColor)
{
  ZoneScopedN("Renderer2D::DrawQuad(transform, texture)");// NOLINT
  if (TriangleCount() + 2 > Data.TrianglesPerBatch) { NextBatch(); }

  auto textureIndex = PushTexture(texture);

  auto vertexOffset = Data.TriangleVertices.size();

  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[0], tintColor, QUAD_UV_COORDINATES[0], textureIndex);
  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[1], tintColor, QUAD_UV_COORDINATES[1], textureIndex);
  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[2], tintColor, QUAD_UV_COORDINATES[2], textureIndex);
  Data.TriangleVertices.emplace_back(
    transform * QUAD_VERTEX_POSITION[3], tintColor, QUAD_UV_COORDINATES[3], textureIndex);

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