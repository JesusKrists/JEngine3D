
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/ILayer.hpp"

#include <stdint.h>// for uint32_t
#include <iterator>// for begin, end, rbegin, rend

namespace JE {


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class LayerStack
{

  using LayerContainer = Vector<Scope<ILayer, MemoryTag::App>, MemoryTag::App>;

public:
  ~LayerStack();

  template<typename DerivedLayer, typename... Args> inline auto PushLayer(Args &&...args) -> DerivedLayer &
  {
    auto layerIt = m_Layers.insert(std::begin(m_Layers) + m_LayerInsertIndex++,
      CreatePolymorphicScope<DerivedLayer, MemoryTag::App, ILayer>(std::forward<Args>(args)...));
    (*layerIt)->OnCreate();
    return static_cast<DerivedLayer &>(*(layerIt->get()));
  }
  template<typename DerivedLayer, typename... Args> inline auto PushOverlay(Args &&...args) -> DerivedLayer &
  {
    m_Layers.push_back(CreatePolymorphicScope<DerivedLayer, MemoryTag::App, ILayer>(std::forward<Args>(args)...));
    auto &layer = m_Layers.back();
    layer->OnCreate();
    return static_cast<DerivedLayer &>(*layer);
  }

  void PopLayer(ILayer &layer);
  void PopOverlay(ILayer &layer);


  [[nodiscard]] inline auto begin() -> LayerContainer::iterator { return std::begin(m_Layers); }
  [[nodiscard]] inline auto end() -> LayerContainer::iterator { return std::end(m_Layers); }
  [[nodiscard]] inline auto rbegin() -> LayerContainer::reverse_iterator { return std::rbegin(m_Layers); }
  [[nodiscard]] inline auto rend() -> LayerContainer::reverse_iterator { return std::rend(m_Layers); }


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