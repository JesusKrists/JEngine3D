
#include "JEngine3D/Core/MemoryController.hpp"

#include <stdint.h>// for uint32_t
#include <functional>// for reference_wrapper
#include <iterator>// for begin, end, rbegin, rend

namespace JE {

class ILayer;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class LayerStack
{
public:
  ~LayerStack();

  void PushLayer(ILayer &layer);
  void PushOverlay(ILayer &layer);

  void PopLayer(ILayer &layer);
  void PopOverlay(ILayer &layer);


  inline auto begin() -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::iterator
  {
    return std::begin(m_Layers);
  }
  inline auto end() -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::iterator { return std::end(m_Layers); }
  inline auto rbegin() -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::reverse_iterator
  {
    return std::rbegin(m_Layers);
  }
  inline auto rend() -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::reverse_iterator
  {
    return std::rend(m_Layers);
  }


  [[nodiscard]] inline auto begin() const -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::const_iterator
  {
    return std::begin(m_Layers);
  }
  [[nodiscard]] inline auto end() const -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::const_iterator
  {
    return std::end(m_Layers);
  }
  [[nodiscard]] inline auto rbegin() const
    -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::const_reverse_iterator
  {
    return std::rbegin(m_Layers);
  }
  [[nodiscard]] inline auto rend() const
    -> Vector<std::reference_wrapper<ILayer>, MemoryTag::App>::const_reverse_iterator
  {
    return std::rend(m_Layers);
  }

  [[nodiscard]] inline auto Layers() const -> const Vector<std::reference_wrapper<ILayer>, MemoryTag::App> &
  {
    return m_Layers;
  }

private:
  Vector<std::reference_wrapper<ILayer>, MemoryTag::App> m_Layers;
  uint32_t m_LayerInsertIndex = 0;
};

}// namespace JE