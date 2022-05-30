#pragma once

struct ImDrawData;
struct ImDrawList;

namespace JE {


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ImGuiRenderer
{
public:
  ImGuiRenderer();
  ~ImGuiRenderer();

  void RenderDrawData(ImDrawData *drawData);

private:
  void RenderCommandList(const ImDrawList *drawList);
};


}// namespace JE