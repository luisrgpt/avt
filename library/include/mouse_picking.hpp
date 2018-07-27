/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MOUSE
#define MOUSE

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <optional>

#include "graph_scene.hpp"
#include "math/matrix.hpp"

namespace gl {
  std::optional<node>* search_in(graph<model>, float, float);
};

#endif