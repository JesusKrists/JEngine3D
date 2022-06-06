#include "Renderer2D.hpp"


#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"
#include "JEngine3D/Core/WindowController.hpp"// for Window

#include <JEngine3D/Renderer/IRendererObjectCreator.hpp>
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
  "uniform sampler2D u_Textures[32];\n"
  "void main()\n"
  "{\n"
  "    if (Frag_TexIndex != -1) {\n"
  "        switch (Frag_TexIndex) {\n"
  "            case  0: Out_Color = Frag_Color * texture(u_Textures[0], Frag_UV); break;\n"
  "            case  1: Out_Color = Frag_Color * texture(u_Textures[1], Frag_UV); break;\n"
  "            case  2: Out_Color = Frag_Color * texture(u_Textures[2], Frag_UV); break;\n"
  "            case  3: Out_Color = Frag_Color * texture(u_Textures[3], Frag_UV); break;\n"
  "            case  4: Out_Color = Frag_Color * texture(u_Textures[4], Frag_UV); break;\n"
  "            case  5: Out_Color = Frag_Color * texture(u_Textures[5], Frag_UV); break;\n"
  "            case  6: Out_Color = Frag_Color * texture(u_Textures[6], Frag_UV); break;\n"
  "            case  7: Out_Color = Frag_Color * texture(u_Textures[7], Frag_UV); break;\n"
  "            case  8: Out_Color = Frag_Color * texture(u_Textures[8], Frag_UV); break;\n"
  "            case  9: Out_Color = Frag_Color * texture(u_Textures[9], Frag_UV); break;\n"
  "            case 10: Out_Color = Frag_Color * texture(u_Textures[10], Frag_UV); break;\n"
  "            case 11: Out_Color = Frag_Color * texture(u_Textures[11], Frag_UV); break;\n"
  "            case 12: Out_Color = Frag_Color * texture(u_Textures[12], Frag_UV); break;\n"
  "            case 13: Out_Color = Frag_Color * texture(u_Textures[13], Frag_UV); break;\n"
  "            case 14: Out_Color = Frag_Color * texture(u_Textures[14], Frag_UV); break;\n"
  "            case 15: Out_Color = Frag_Color * texture(u_Textures[15], Frag_UV); break;\n"
  "            case 16: Out_Color = Frag_Color * texture(u_Textures[16], Frag_UV); break;\n"
  "            case 17: Out_Color = Frag_Color * texture(u_Textures[17], Frag_UV); break;\n"
  "            case 18: Out_Color = Frag_Color * texture(u_Textures[18], Frag_UV); break;\n"
  "            case 19: Out_Color = Frag_Color * texture(u_Textures[19], Frag_UV); break;\n"
  "            case 20: Out_Color = Frag_Color * texture(u_Textures[20], Frag_UV); break;\n"
  "            case 21: Out_Color = Frag_Color * texture(u_Textures[21], Frag_UV); break;\n"
  "            case 22: Out_Color = Frag_Color * texture(u_Textures[22], Frag_UV); break;\n"
  "            case 23: Out_Color = Frag_Color * texture(u_Textures[23], Frag_UV); break;\n"
  "            case 24: Out_Color = Frag_Color * texture(u_Textures[24], Frag_UV); break;\n"
  "            case 25: Out_Color = Frag_Color * texture(u_Textures[25], Frag_UV); break;\n"
  "            case 26: Out_Color = Frag_Color * texture(u_Textures[26], Frag_UV); break;\n"
  "            case 27: Out_Color = Frag_Color * texture(u_Textures[27], Frag_UV); break;\n"
  "            case 28: Out_Color = Frag_Color * texture(u_Textures[28], Frag_UV); break;\n"
  "            case 29: Out_Color = Frag_Color * texture(u_Textures[29], Frag_UV); break;\n"
  "            case 30: Out_Color = Frag_Color * texture(u_Textures[30], Frag_UV); break;\n"
  "            case 31: Out_Color = Frag_Color * texture(u_Textures[31], Frag_UV); break;\n"
  "        }\n"
  "    } else {\n"
  "        Out_Color = Frag_Color;\n"
  "    }\n"
  "}\n";

Renderer2D::Renderer2D()
{
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
  ASSERT(Data.Target != nullptr, "IDrawTarget is missing");
  ASSERT(Data.TriangleIndices.size() != 0, "Nothing has been drawn");

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
    Data.Shader->Bind();
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
  auto *target = Data.Target;
  Flush();
  InitializeBatch(target);
}

auto Renderer2D::PushTexture(const ITexture &texture) -> int8_t
{
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

  auto textureIndex = PushTexture(texture);

  vertex0.TextureIndex = textureIndex;// NOLINT
  vertex1.TextureIndex = textureIndex;// NOLINT
  vertex2.TextureIndex = textureIndex;// NOLINT

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