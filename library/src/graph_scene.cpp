#include "graph_scene.hpp"

using namespace gl;

model::model(
  scene scene,
  math::vector_3d scale,
  math::vector_3d position,
  math::quaternion orientation,
  math::vector_3d velocity,
  math::quaternion rotation)
  : scene_info(scene)
  , object(position, orientation, velocity, rotation)
  , scale(math::matrix_4d::Scaling(scale.x, scale.y, scale.z, 1.0f)) {
}

void model::execute(kinematic_movement movement) {
  this->object.execute(movement);
}

template<class type>
graph<type>::graph()
  : root(node{ 0u })
  , counter(1u)
  , nodes({ std::optional<type>() })
  , edges({ this->root }) {}

template<class type>
node* graph<type>::set_root(type content) {
  node* root = new node{ this->counter++ };
  this->nodes.push_back(std::optional(content));
  this->edges.push_back(this->root);

  return root;
}
template<class type>
node* graph<type>::set_child(node parent, type content) {
  node* child = new node{ this->counter++ };
  this->nodes.push_back(content);
  this->edges.push_back(parent);

  return child;
}
template<class type>
type& graph<type>::get(node node) {
  return nodes[node.id].value();
}

template gl::graph<gl::model>::graph();
template gl::node* gl::graph<gl::model>::set_root(gl::model);
template gl::node* gl::graph<gl::model>::set_child(gl::node, gl::model);
template gl::model& gl::graph<gl::model>::get(gl::node);

graph<model>* gl::bltz::load(std::string filename)
{
  graph<model>* scene_graph = new graph<model>();
  scene scene = std::vector<mesh>();
  unsigned p = 0;
  math::vector_3d t = math::vector_3d::kZero;
  math::quaternion r = math::quaternion::kZero;
  math::vector_3d s = math::vector_3d::kZero;
  bool l = false;
  vertex_type v = v_none;
  fragment_type f = f_none;
  size_t n = 0;
  std::string ff = "";
  std::string fv = "";
  std::string fo = "";
  std::string fm = "";
  unsigned o0 = 0;
  unsigned o1 = 0;
  unsigned o2 = 0;
  unsigned o3 = 0;
  unsigned o4 = 0;
  unsigned o5 = 0;
  bool o6 = 0;
  std::array<float, 3> o7 = std::array<float, 3>();
  std::array<float, 3> o8 = std::array<float, 3>();
  std::array<float, 3> o9 = std::array<float, 3>();
  std::array<float, 3> oa = std::array<float, 3>();
  float ob = 0;
  int oc = 0;

  std::ifstream file(filename);
  std::string line;
  std::string token;
  std::deque<std::string> tokens;
  std::string prefix;
  while (std::getline(file, line)) {
    auto sin = std::stringstream(line);
    tokens.clear();
    while (std::getline(sin, token, ' ')) {
      tokens.push_back(token);
    }
    if (tokens.size() == 0) {
      continue;
    }
    prefix = tokens.front();
    tokens.pop_front();
    if (prefix.compare("p") == 0) {
      p = std::stoi(tokens[0]);
    }
    else if (prefix.compare("t") == 0) {
      t = math::vector_3d(
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      );
    }
    else if (prefix.compare("r") == 0) {
      r = math::quaternion(
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2]),
        std::stof(tokens[3])
      );
    }
    else if (prefix.compare("s") == 0) {
      s = math::vector_3d(
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      );
    }
    else if (prefix.compare("l") == 0) {
      l = std::stoi(tokens[0]) == 1;

      if (l) {
        scene.push_back(
          mesh { true }
        );
      }
    }
    else if (prefix.compare("v") == 0) {
      v = static_cast<vertex_type>(std::stoi(tokens[0]));
    }
    else if (prefix.compare("f") == 0) {
      f = static_cast<fragment_type>(std::stoi(tokens[0]));
    }
    else if (prefix.compare("n") == 0) {
      n = std::stoi(tokens[0]);
    }
    else if (prefix.compare("ff") == 0) {
      ff = tokens[0];
    }
    else if (prefix.compare("fv") == 0) {
      fv = tokens[0];
    }
    else if (prefix.compare("fo") == 0) {
      fo = tokens[0];
    }
    else if (prefix.compare("fm") == 0) {
      fm = tokens[0];
    }
    else if (prefix.compare("o0") == 0) {
      o0 = std::stoi(tokens[0]);
    }
    else if (prefix.compare("o1") == 0) {
      o1 = std::stoi(tokens[0]);
    }
    else if (prefix.compare("o2") == 0) {
      o2 = std::stoi(tokens[0]);
    }
    else if (prefix.compare("o3") == 0) {
      o3 = std::stoi(tokens[0]);
    }
    else if (prefix.compare("o4") == 0) {
      o4 = std::stoi(tokens[0]);
    }
    else if (prefix.compare("o5") == 0) {
      o5 = std::stof(tokens[0]);
    }
    else if (prefix.compare("o6") == 0) {
      o6 = std::stoi(tokens[0]);

      if (!o6) {
        scene.push_back(
          mesh{
            false,
            v,
            f,
            fv,
            ff,
            fo,
            fm,

            o0,
            n,
            o1,
            o2,
            o3,
            o4,
            o5,
            o6
          }
        );
      }
    }
    else if (prefix.compare("o7") == 0) {
      o7 = {
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      };
    }
    else if (prefix.compare("o8") == 0) {
      o8 = {
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      };
    }
    else if (prefix.compare("o9") == 0) {
      o9 = {
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      };
    }
    else if (prefix.compare("oa") == 0) {
      oa = {
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      };
    }
    else if (prefix.compare("ob") == 0) {
      ob = std::stoi(tokens[0]);
    }
    else if (prefix.compare("oc") == 0) {
      oc = std::stoi(tokens[0]);

      scene.push_back(
        mesh{
          false,
          v,
          f,
          fv,
          ff,
          fo,
          fm,

          o0,
          n,
          o1,
          o2,
          o3,
          o4,
          o5,
          o6,
          o7,
          o8,
          o9,
          oa,
          ob,
          oc
        }
      );
    }
    else if (prefix.compare("x") == 0) {
      scene_graph->set_child(
        node { p },
        model(
          scene, s, t, r, math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        )
      );
      scene.clear();
    }
  }

  return scene_graph;
}

void bltz::save(graph<model> scene_graph, std::string filename)
{
  std::ofstream file(filename, std::ios::binary);

  for (auto i = 1u; i < scene_graph.nodes.size(); i++) {
    auto node = scene_graph.nodes[i].value();
    auto parent_id = scene_graph.edges[i].id;

    file << "p " << scene_graph.edges[i].id << std::endl;
    file << "t " << node.object.position.x << " " << node.object.position.y << " " << node.object.position.z << std::endl;
    file << "r " << node.object.orientation.x << " " << node.object.orientation.y << " " << node.object.orientation.z << " " << node.object.orientation.w << std::endl;
    file << "s " << node.scale[0][0] << " " << node.scale[1][1] << " " << node.scale[2][2] << std::endl;
    for (auto mesh : scene_graph.nodes[i].value().scene_info) {
      file << "l " << mesh.is_light << std::endl;
      if (mesh.is_light) {
        continue;
      }
      file << "v " << mesh.vertex_type << std::endl;
      file << "f " << mesh.fragment_type << std::endl;
      file << "n " << mesh.n_faces << std::endl;

      file << "fv " << mesh.vertex_file << std::endl;
      file << "ff " << mesh.fragment_file << std::endl;
      file << "fo " << mesh.object_file << std::endl;
      file << "fm " << mesh.material_file << std::endl;

      file << "o0 " << mesh.id << std::endl;
      file << "o1 " << mesh.program_id << std::endl;
      file << "o2 " << mesh.texture_id << std::endl;
      file << "o3 " << mesh.block_id << std::endl;
      file << "o4 " << mesh.vertex << std::endl;
      file << "o5 " << mesh.fragment << std::endl;

      file << "o6 " << mesh.has_material << std::endl;
      file << "o7 " << mesh.ambient[0] << " " << mesh.ambient[1] << " " << mesh.ambient[2] << std::endl;
      file << "o8 " << mesh.diffuse[0] << " " << mesh.diffuse[1] << " " << mesh.diffuse[2] << std::endl;
      file << "o9 " << mesh.specular[0] << " " << mesh.specular[1] << " " << mesh.specular[2] << std::endl;
      file << "oa " << mesh.emissive[0] << " " << mesh.emissive[1] << " " << mesh.emissive[2] << std::endl;
      file << "ob " << mesh.shininess << std::endl;
      file << "oc " << mesh.n_textures << std::endl;
    }
    file << "x" << std::endl;
  }

  file.close();
}
