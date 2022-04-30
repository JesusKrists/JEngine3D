
#include "JEngine3D/Core/MemoryController.hpp"

#include <stdint.h>// for uint32_t
#include <functional>// for reference_wrapper
#include <iterator>// for begin, end, rbegin, rend

namespace JE {

class ILayer;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class LayerStack
{

  using LayerContainer = Vector<std::reference_wrapper<ILayer>, MemoryTag::App>;

public:
  ~LayerStack();

  void PushLayer(ILayer &layer);
  void PushOverlay(ILayer &layer);

  void PopLayer(ILayer &layer);
  void PopOverlay(ILayer &layer);


  inline auto begin() -> LayerContainer::iterator { return std::begin(m_Layers); }
  inline auto end() -> LayerContainer::iterator { return std::end(m_Layers); }
  inline auto rbegin() -> LayerContainer::reverse_iterator { return std::rbegin(m_Layers); }
  inline auto rend() -> LayerContainer::reverse_iterator { return std::rend(m_Layers); }


  [[nodiscard]] inline auto begin() const -> LayerContainer::const_iterator { return std::begin(m_Layers); }
  [[nodiscard]] inline auto end() const -> LayerContainer::const_iterator { return std::end(m_Layers); }
  [[nodiscard]] inline auto rbegin() const -> LayerContainer::const_reverse_iterator { return std::rbegin(m_Layers); }
  [[nodiscard]] inline auto rend() const -> LayerContainer::const_reverse_iterator { return std::rend(m_Layers); }

  [[nodiscard]] inline auto Layers() const -> const LayerContainer & { return m_Layers; }

private:
  LayerContainer m_Layers;
  uint32_t m_LayerInsertIndex = 0;
};

}// namespace JE