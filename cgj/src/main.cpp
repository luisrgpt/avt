#include <iostream>
#include <functional>

#include <glm/gtc/type_ptr.hpp>

#include "math/quaternion.hpp"
#include "math/vector_3d.hpp"
#include "engine.hpp"
#include "graph_scene.hpp"
#include "mouse_picking.hpp"

constexpr unsigned
fps = 60,
mhz = 1000 / fps,
width = 1000,
height = 800;

gl::engine *engine;
gl::program *program;
gl::graph<gl::model> *scene_graph;
std::vector<gl::camera> cameras;
std::vector<gl::free_camera> free_cameras;
fs::mtl *random_materials;
gl::node *node_field;
gl::node *node_light;
gl::node *picked_node;
unsigned current_projection;
unsigned current_view;
unsigned current_mode;
unsigned current_material;

/////////////////////////////////////////////////////////////////////// CALLBACKS

void close() {
  delete engine;
}
void idle() {}
void keyboard(unsigned char key, int x, int y) {
  int r1 = std::rand() % (scene_graph->nodes.size() - 1) + 1;
  int r2 = std::rand() % random_materials->Ka.size();

  switch (key) {
  case 'x':
    delete engine;
    break;
  case 'b':
    gl::bltz::save(*scene_graph, "test.bltz");
    break;
  case 'r':
    scene_graph->nodes[r1].value().scene_info.at(0).ambient = random_materials->Ka[r2];
    scene_graph->nodes[r1].value().scene_info.at(0).diffuse = random_materials->Kd[r2];
    scene_graph->nodes[r1].value().scene_info.at(0).emissive = random_materials->Ke[r2];
    break;
  case 'n':
    scene_graph = gl::bltz::load("test.bltz");
    break;
  case 'g':
    if (current_view == 0) {
      current_view = 1;
    }
    else {
      current_view = 0;
    }
    break;
  case 'p':
    if (current_projection == 0) {
      current_projection = 1;
    }
    else {
      current_projection = 0;
    }
    break;
  case 'm':
    if (current_mode == 0) {
      current_mode = 1;
    }
    else {
      current_mode = 0;
    }
    break;

  case 'a':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(-1.0f, 0.0f, 0.0f), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_velocity(math::vector_3d(-1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.position += math::vector_3d(-1.0f, 0.0f, 0.0f);
    }
    break;
  case 'd':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_velocity(math::vector_3d(1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.position += math::vector_3d(1.0f, 0.0f, 0.0f);
    }
    break;
  case 's':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, -1.0f, 0.0f), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_velocity(math::vector_3d(0.0f, -1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.position += math::vector_3d(0.0f, -1.0f, 0.0f);
    }
    break;
  case 'w':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_velocity(math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.position += math::vector_3d(0.0f, 1.0f, 0.0f);
    }
    break;
  case 'q':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_velocity(math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.position += math::vector_3d(0.0f, 0.0f, -1.0f);
    }
    break;
  case 'e':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_velocity(math::vector_3d(0.0f, 0.0f, 1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.position += math::vector_3d(0.0f, 0.0f, 1.0f);
    }
    break;

  case '1':
    current_material = 0;
    break;
  case '2':
    current_material = 1;
    break;
  case '3':
    current_material = 2;
    break;
  case '4':
    current_material = 3;
    break;
  case '5':
    current_material = 4;
    break;
  case '6':
    current_material = 5;
    break;
  case '7':
    current_material = 6;
    break;
  case '8':
    current_material = 7;
    break;

  case 'j':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_rotation(math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
    }
    break;
  case 'l':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, -1.0f, 0.0f)), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_rotation(math::vector_3d(0.0f, -1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, -1.0f, 0.0f)), 1));
    }
    break;
  case 'k':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(-1.0f, 0.0f, 0.0f)), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_rotation(math::vector_3d(-1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(-1.0f, 0.0f, 0.0f)), 1));
    }
    break;
  case 'i':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_rotation(math::vector_3d(1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
    }
    break;
  case 'u':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f)), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_rotation(math::vector_3d(0.0f, 0.0f, 1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, 0.0f, 1.0f)), 1));
    }
    break;
  case 'o':
    if (current_mode == 0) {
      for (auto &camera : cameras) {
        camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f)), 1));
      }
      for (auto &camera : free_cameras) {
        camera.view.apply_rotation(math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
      }
    }
    else {
      scene_graph->nodes[picked_node->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, 0.0f, -1.0f)), 1));
    }
    break;
    }
}

float state_x;
float state_y;
bool got_picked = false;
void motion(int x, int y) {
  if (got_picked) {
    float normalized_x = (2.0f * x) / engine->width - 1.0f;
    float normalized_y = 1.0f - (2.0f * y) / engine->height;

    glm::vec4 nv = glm::vec4(normalized_x, normalized_y, -1.0f, 1.0f);
    glm::mat4 projection;
    glm::mat4 view;
    if (current_view == 0) {
      projection = glm::make_mat4(cameras[current_projection].projection.matrix.values.data());
      view = glm::make_mat4(cameras[current_projection].view.matrix.values.data());
    }
    else {
      projection = glm::make_mat4(free_cameras[current_projection].projection.matrix.values.data());
      view = glm::make_mat4(free_cameras[current_projection].view.matrix.values.data());
    }

    nv = glm::inverse(view) * glm::inverse(projection) * nv;
    auto nx = nv.x - state_x;
    auto ny = nv.y - state_y;
    state_x = nv.x;
    state_y = nv.y;

    scene_graph->nodes[picked_node->id].value().object.execute(gl::linear_movement(math::vector_3d(nx, ny, 0.0f), 1));
  }
}
void mouse(int button, int state, int x, int y) {
  bool left_is_down = engine->left_button_is_down(button, state);
  bool right_is_down = engine->right_button_is_down(button, state);

  if (left_is_down || right_is_down) {
    for (auto &camera : cameras) {
      camera.view.reset();
    }
    for (auto &camera : free_cameras) {
      camera.view.reset();
    }

    float normalized_x = (2.0f * x) / engine->width - 1.0f;
    float normalized_y = 1.0f - (2.0f * y) / engine->height;

    glm::vec4 nv = glm::vec4(normalized_x, normalized_y, -1.0f, 1.0f);
    glm::mat4 projection;
    glm::mat4 view;
    if (current_view == 0) {
      projection = glm::make_mat4(cameras[current_projection].projection.matrix.values.data());
      view = glm::make_mat4(cameras[current_projection].view.matrix.values.data());
    }
    else {
      projection = glm::make_mat4(free_cameras[current_projection].projection.matrix.values.data());
      view = glm::make_mat4(free_cameras[current_projection].view.matrix.values.data());
    }

    nv = glm::inverse(view) * glm::inverse(projection) * nv;
    auto nx = nv.x;
    auto ny = nv.y;

    auto maybe_node = gl::search_in(*scene_graph, nx, ny);

    if (maybe_node->has_value()) {
      unsigned id = maybe_node->value().id;
      if (left_is_down) {
        picked_node = new gl::node{ id };
        got_picked = true;
        state_x = nx;
        state_y = ny;
      } else {
        scene_graph->nodes[id].value().scene_info.at(0).ambient = random_materials->Ka[current_material];
        scene_graph->nodes[id].value().scene_info.at(0).diffuse = random_materials->Kd[current_material];
        scene_graph->nodes[id].value().scene_info.at(0).emissive = random_materials->Ke[current_material];
      }
    }
  }
  else if (engine->left_button_is_up(button, state)) {
    got_picked = false;
    state_x = 0;
    state_y = 0;
  }
}
void display() {
  if (current_view == 0) {
    engine->draw(
      *scene_graph,
      cameras[current_projection]
    );
  }
  else {
    engine->draw(
      *scene_graph,
      free_cameras[current_projection]
    );
  }
}
int cnt = 0;
void timer(int value) {
  engine->set_timer_callback(mhz, timer, 0);
  engine->display();
}
void reshape(int width, int height) {
  engine->reshape(width, height);
  cameras[current_projection].projection.reshape(width, height);
}

int main(int argc, char **argv) {
  engine = new gl::engine(&argc, argv, "Arkanoid Level Editor", width, height);
  engine->set_close_callback(close);
  engine->set_display_callback(display);
  engine->set_idle_callback(idle);
  engine->set_reshape_callback(reshape);
  engine->set_keyboard_callback(keyboard);
  engine->set_motion_callback(motion);
  engine->set_mouse_callback(mouse);
  engine->set_timer_callback(0, timer, 0);

  program = engine->create_program();
  engine->load_shader<gl::vertex>(*program, "share/texture.vert");
  engine->load_shader<gl::fragment>(*program, "share/texture.frag");
  engine->link(*program);

  random_materials = new fs::mtl("share/random_materials.mtl");

  fs::mtl materials("share/materials.mtl");
  fs::obj obj_field("share/field.obj");
  fs::obj obj_ship("share/ship.obj");
  fs::obj obj_ball("share/ball.obj");
  fs::obj obj_box("share/box.obj");
  fs::obj obj_pipe("share/big_pipe.obj");
  fs::obj obj_border("share/border.obj");

  gl::scene *field = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*program, obj_field, materials);
  gl::scene *ship = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*program, obj_ship, materials);
  gl::scene *ball = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*program, obj_ball, materials);
  gl::scene *box = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*program, obj_box, materials);
  gl::scene *big_pipe = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*program, obj_pipe, materials);
  gl::scene *border = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*program, obj_border, materials);

  scene_graph = new gl::graph<gl::model>();
  node_field = scene_graph->set_root(
    gl::model(
      *field,
      math::vector_3d(1, 1, 1),
      math::vector_3d(-23.5f, 14.5f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  node_light = scene_graph->set_child(
    *node_field,
    gl::model(
      gl::light,
      math::vector_3d(0, 0, 0),
      math::vector_3d(0.0f, 0.0f, 0.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  picked_node = scene_graph->set_child(
    *node_field,
    gl::model(
      *ship,
      math::vector_3d(1, 1, 1),
      math::vector_3d(0.0f, -12.0f, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  scene_graph->set_child(
    *node_field,
    gl::model(
      *ball,
      math::vector_3d(0.6f, 0.6f, 0.6f),
      math::vector_3d(0.0f, -11.0f, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  for (auto i = 0u; i < 6u; i++) {
    for (auto j = 0u; j < 13u; j++) {
      scene_graph->set_child(
        *node_field,
        gl::model(
          *box,
          math::vector_3d(1.8f, 0.8f, 0.8f),
          math::vector_3d(-11.5f + 1.9f * j, 3.95f + 0.85f * i, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        )
      );
    }
  }
  scene_graph->set_child(
    *node_field,
    gl::model(
      *border,
      math::vector_3d(1, 1, 1),
      math::vector_3d(0.0f, -0.2f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  for (auto i = 0u; i < 6u; i++) {
    for (auto j = 0u; j < 2u; j++) {
      scene_graph->set_child(
        *node_field,
        gl::model(
          *big_pipe,
          math::vector_3d(1, 1, 1),
          math::vector_3d(-13.0f + 26.0f * j, -12.625f + 5.0f * i, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        )
      );
    }
  }
  for (auto j = 0u; j < 2u; j++) {
    scene_graph->set_child(
      *node_field,
      gl::model(
        *big_pipe,
        math::vector_3d(1, 1, 1),
        math::vector_3d(-6.75f + 13.5f * j, 14.5f, 0.0f), math::quaternion(90, math::vector_3d(0.0f, 0.0f, 1.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      )
    );
  }

  cameras = {
    gl::camera{
      gl::view(
        math::vector_3d(-23.5f, 15.0f, 30.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      ),
      gl::perspective_projection(
        width, height,
        20,
        1.0f, 100.0f
      )
    },
    gl::camera{
      gl::view(
        math::vector_3d(-23.5f, 15.0f, 30.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      ),
      gl::orthogonal_projection(
        width, height,
        -13.5f, 13.5f,
        -15.0f, 15.0f,
        1.0f, 100.0f
      )
    }
  };

  free_cameras = {
    gl::free_camera{
      gl::free_view(
        math::vector_3d(-23.5f, 15.0f, 30.0f),
        math::vector_3d(0, 0, 0)
      ),
      gl::perspective_projection(
        width, height,
        20,
        1.0f, 100.0f
      )
    },
    gl::free_camera{
      gl::free_view(
        math::vector_3d(-23.5f, 15.0f, 30.0f),
        math::vector_3d(0, 0, 0)
      ),
      gl::orthogonal_projection(
        width, height,
        -13.5f, 13.5f,
        -15.0f, 15.0f,
        1.0f, 100.0f
      )
    }
  };
  current_projection = 1;
  current_view = 0;
  current_mode = 0;

  engine->start();

  return (0);
}