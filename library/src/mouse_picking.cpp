#include "mouse_picking.hpp"

using namespace gl;

std::optional<node>* gl::search_in(graph<model> graph, float x, float y) {
  std::optional<node> *top_node = new std::optional<node>();
  auto tz = 0.0f;


  std::vector<math::matrix_4d> translations = { math::matrix_4d::kIdentity };
  std::vector<math::matrix_4d> rotations = { math::matrix_4d::kIdentity };
  std::vector<math::matrix_4d> scalations = { math::matrix_4d::kIdentity };

  // Calculate matrices
  for (auto i = 1u; i < graph.nodes.size(); i++) {
    auto node = graph.nodes[i].value();
    auto parent_id = graph.edges[i].id;

    translations.push_back(math::matrix_3d::Translation(node.object.position.x, node.object.position.y, node.object.position.z) * translations[parent_id]);
    rotations.push_back(math::matrix_4d(node.object.orientation) * rotations[parent_id]);
    scalations.push_back(node.scale * scalations[parent_id]);
  }


  for (auto idx = 0u; idx < graph.nodes.size(); idx++) {
    auto maybe_node = graph.nodes[idx];

    if (!maybe_node.has_value()) {
      continue;
    }

    auto px = translations[idx][3][0];
    auto py = translations[idx][3][1];
    auto pz = translations[idx][3][2];
    auto sx = maybe_node.value().scale[0][0];
    auto sy = maybe_node.value().scale[1][1];
    if (px - sx < x && x < px + sx && py - sy < y && y < py + sy && (!top_node->has_value() || pz < tz)) {
    std::cout << px << " " << py << std::endl;
      top_node = new std::optional<node>(node{ idx });
      tz = pz;
    }
  }

  return top_node;
}
