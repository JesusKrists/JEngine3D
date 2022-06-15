#include "LayerStack.hpp"

#include "JEngine3D/Core/Base.hpp"// for find_if, ForEach
#include "JEngine3D/Core/ILayer.hpp"// for ILayer

namespace JE {


LayerStack::~LayerStack()
{
  ReverseForEach(m_Layers, [](Scope<ILayer, MemoryTag::App> &layer) { layer->OnDestroy(); });
}

void LayerStack::PopLayer(ILayer &layer)
{
  auto layerIt = std::find_if(std::begin(m_Layers),
    std::begin(m_Layers) + m_LayerInsertIndex,
    [&](const Scope<ILayer, MemoryTag::App> &layerEntry) { return layerEntry.get() == &layer; });
  if (layerIt != std::end(m_Layers)) {
    layer.OnDestroy();
    m_Layers.erase(layerIt);
    --m_LayerInsertIndex;
  }
}

void LayerStack::PopOverlay(ILayer &layer)
{
  auto layerIt = std::find_if(std::begin(m_Layers) + m_LayerInsertIndex,
    std::end(m_Layers),
    [&](const Scope<ILayer, MemoryTag::App> &layerEntry) { return layerEntry.get() == &layer; });
  if (layerIt != std::end(m_Layers)) {
    layer.OnDestroy();
    m_Layers.erase(layerIt);
  }
}

}// namespace JE