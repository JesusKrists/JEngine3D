#include "LayerStack.hpp"

#include "JEngine3D/Core/Base.hpp"// for find_if, ForEach
#include "JEngine3D/Core/ILayer.hpp"// for ILayer

namespace JE {


LayerStack::~LayerStack()
{
  ForEach(m_Layers, [](ILayer &layer) { layer.OnDestroy(); });
}

void LayerStack::PushLayer(ILayer &layer)
{
  m_Layers.emplace(std::begin(m_Layers) + m_LayerInsertIndex++, layer);
  layer.OnCreate();
}

void LayerStack::PushOverlay(ILayer &layer)
{
  m_Layers.emplace_back(layer);
  layer.OnCreate();
}

void LayerStack::PopLayer(ILayer &layer)
{
  auto layerIt = std::find_if(std::begin(m_Layers),
    std::begin(m_Layers) + m_LayerInsertIndex,
    [&](const ILayer &layerEntry) { return &layerEntry == &layer; });
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
    [&](const ILayer &layerEntry) { return &layerEntry == &layer; });
  if (layerIt != std::end(m_Layers)) {
    layer.OnDestroy();
    m_Layers.erase(layerIt);
  }
}

}// namespace JE