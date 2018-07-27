#include "mouse_picking.hpp"

using namespace gl;

std::optional<node>* gl::search_in(graph<model> graph, float x, float y) {
  std::optional<node> *top_node;
  auto tz = 0.0f;

  for (auto idx = 0u; idx < graph.nodes.size(); idx++) {
    auto maybe_node = graph.nodes[idx];

    if (!maybe_node.has_value()) {
      continue;
    }

    auto px = maybe_node.value().object.position.x;
    auto py = maybe_node.value().object.position.y;
    auto pz = maybe_node.value().object.position.z;
    auto sx = maybe_node.value().scale[0][0];
    auto sy = maybe_node.value().scale[1][1];

    if (px - sx < x && x < px + sx && py - sy < y && y < py + sy && (!top_node->has_value() || pz < tz)) {
      top_node = new std::optional<node>(node{ idx });
      tz = pz;
    }
  }

  return top_node;
}
