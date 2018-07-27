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
width = 640,
height = 480;

gl::engine *engine;
gl::program *program;
gl::graph<gl::model> *scene_graph;
std::vector<gl::camera> cameras;
std::vector<gl::free_camera> free_cameras;
gl::node *node_field;
gl::node *node_light;
unsigned current_projection;
unsigned current_view;

/////////////////////////////////////////////////////////////////////// CALLBACKS

void close() {
  delete engine;
}
void idle() {}
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
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
  case 'a':
    for (auto &camera : cameras) {
      camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(-1.0f, 0.0f, 0.0f), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_velocity(math::vector_3d(-1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'd':
    for (auto &camera : cameras) {
      camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_velocity(math::vector_3d(1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 's':
    for (auto &camera : cameras) {
      camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, -1.0f, 0.0f), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_velocity(math::vector_3d(0.0f, -1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'w':
    for (auto &camera : cameras) {
      camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_velocity(math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'q':
    for (auto &camera : cameras) {
      camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_velocity(math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'e':
    for (auto &camera : cameras) {
      camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_velocity(math::vector_3d(0.0f, 0.0f, 1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;

  case '4':
    scene_graph->nodes[node_field->id].value().object.position += math::vector_3d(-1.0f, 0.0f, 0.0f);
    break;
  case '6':
    scene_graph->nodes[node_field->id].value().object.position += math::vector_3d(1.0f, 0.0f, 0.0f);
    break;
  case '5':
    scene_graph->nodes[node_field->id].value().object.position += math::vector_3d(0.0f, -1.0f, 0.0f);
    break;
  case '8':
    scene_graph->nodes[node_field->id].value().object.position += math::vector_3d(0.0f, 1.0f, 0.0f);
    break;
  case '7':
    scene_graph->nodes[node_field->id].value().object.position += math::vector_3d(0.0f, 0.0f, -1.0f);
    break;
  case '9':
    scene_graph->nodes[node_field->id].value().object.position += math::vector_3d(0.0f, 0.0f, 1.0f);
    break;

  case 'j':
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'l':
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, -1.0f, 0.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(0.0f, -1.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'k':
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(-1.0f, 0.0f, 0.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(-1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'i':
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(1.0f, 0.0f, 0.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'u':
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(0.0f, 0.0f, 1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  case 'o':
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion(1, camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(camera.view.euler_angle.x, camera.view.euler_angle.y, camera.view.euler_angle.z));
    }
    break;
  }
}

gl::node *picked_node;
bool got_picked = false;
void motion(int x, int y) {
  if (got_picked) {
    std::cout << "KEEP" << std::endl;
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
    auto nx = nv.x - 13.5;
    auto ny = nv.y - 14.5;

    scene_graph->nodes[picked_node->id].value().object.position = math::vector_3d(nx, ny, 1.0f);

    //for (auto &camera : cameras) {
    //camera.view.execute(gl::rotation(math::quaternion((x - x_state) * 10 / (float)fps, camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
    //}
    //for (auto &camera : free_cameras) {
    //  camera.view.apply_rotation(math::vector_3d(0.0f, (x - x_state) * 10 / (float)fps, 0.0f));
    //}
    //for (auto &camera : cameras) {
    //  camera.view.execute(gl::rotation(math::quaternion((y - y_state) * 10 / (float)fps, camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
    //}
    //for (auto &camera : free_cameras) {
    //  camera.view.apply_rotation(math::vector_3d((y - y_state) * 10 / (float)fps, 0.0f, 0.0f));
    //}
  }
}
void mouse(int button, int state, int x, int y) {
  if (engine->left_button_is_down(button, state)) {
    std::cout << "DOWN" << std::endl;
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
    auto nx = nv.x - 13.5;
    auto ny = nv.y - 14.5;
    std::cout << nx << " " << ny << std::endl;

    auto maybe_node = gl::search_in(*scene_graph, nx, ny);

    if (maybe_node->has_value()) {
      picked_node = new gl::node{ maybe_node->value().id };
      got_picked = true;
    }
  }
  else if (engine->left_button_is_up(button, state)) {
    std::cout << "UP" << std::endl;
    got_picked = false;
  }
}
void display() {
  //engine->use(*program);
  //engine->bind(*mesh);
  //engine->set_uniform(
  //  *projection_view_model,
  //  cameras[current_camera].projection.matrix *
  //  cameras[current_camera].view.matrix *
  //  glm::translate(models->at(0).object.position) *
  //  glm::mat4_cast(models->at(0).object.orientation) *
  //  models->at(0).scale
  //);
  //engine->draw(*mesh);
  //engine->unbind();
  //engine->unuse();

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
  std::string title
    = "avt -> " + std::to_string(cnt++);
  //      + std::to_string(fps)
  //      + " FPS @ ("
  //      + std::to_string(window.width)
  //      + "x"
  //      + std::to_string(window.height)
  //      + ")";

  engine->set_title(std::cref(title));
  engine->set_timer_callback(mhz, timer, 0);

  engine->display();
}
void reshape(int width, int height) {
  engine->reshape(width, height);
  cameras[current_projection].projection.reshape(width, height);
}

int main(int argc, char **argv) {
  engine = new gl::engine(&argc, argv, "avt", width, height);
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
      math::vector_3d(13.5f, 14.5f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
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
  scene_graph->set_child(
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
        math::vector_3d(13.5f, 15.0f, 30.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
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
        math::vector_3d(13.5f, 15.0f, 30.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
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
        math::vector_3d(13.5f, 15.0f, 30.0f),
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
        math::vector_3d(13.5f, 15.0f, 30.0f),
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
  current_view = 1;

  //square = engine->load_scene(*program, "share/square.obj");
  //triangle = engine->load_scene(*program, "share/triangle.obj");
  //paralellogram = engine->load_scene(*program, "share/paralellogram.obj");

  //scene_graph = new gl::graph<gl::model>();
  //auto triangle_2_1 = scene_graph->set_root(
  //  gl::model(
  //    *triangle,
  //    math::vector_3d(20, 5, 20),
  //    math::vector_3d(0, 0, 0), math::quaternion(45, math::vector_3d(0.0f, 1.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //auto triangle_2_2 = scene_graph->set_child(
  //  *triangle_2_1,
  //  gl::model(
  //    *triangle,
  //    math::vector_3d(1.0f, 0.5f, 1.0f),
  //    math::vector_3d(0, 0, 0), math::quaternion(180, math::vector_3d(0.0f, 1.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //auto square_1 = scene_graph->set_child(
  //  *triangle_2_1,
  //  gl::model(
  //    *square,
  //    math::vector_3d(0.5f, 0.7f, 0.5f),
  //    math::vector_3d(-7.07107f * 2, 0.0f, -7.07107f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //auto triangle_1_1 = scene_graph->set_child(
  //  *square_1,
  //  gl::model(
  //    *triangle,
  //    math::vector_3d(1.0f, 0.8f, 1.0f),
  //    math::vector_3d(-7.07107f, 0.0f, 7.07107f), math::quaternion(180, math::vector_3d(0.0f, 1.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //auto triangle_1_2 = scene_graph->set_child(
  //  *triangle_2_2,
  //  gl::model(
  //    *triangle,
  //    math::vector_3d(0.5f, 0.5f, 0.5f),
  //    math::vector_3d(1.0f, 0.0f, 7.07107f * 3 - 1.0f), math::quaternion(90, math::vector_3d(0.0f, 1.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //auto triangle_s = scene_graph->set_child(
  //  *triangle_2_2,
  //  gl::model(
  //    *triangle,
  //    math::vector_3d(0.707107f, 2.5f, 0.707107f),
  //    math::vector_3d(7.07107f * 2, 0.0f, 20 * 0.707107f * 0.707107f), math::quaternion(90, math::vector_3d(0.0f, 1.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //auto paralellogram_1 = scene_graph->set_child(
  //  *triangle_s,
  //  gl::model(
  //    *paralellogram,
  //    math::vector_3d(0.5 / 0.707107f, 0.4f, 0.5 / 0.707107f),
  //    math::vector_3d(0.0f, 0.0f, 0.0f), math::quaternion(180, math::vector_3d(1.0f, 0.0f, 0.0f)),
  //    math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //  )
  //);
  //cameras = {
  //  gl::camera{
  //    gl::view(
  //      math::vector_3d(5, 25, 50), math::quaternion(-30, math::vector_3d(1.0f, 0.0f, 0.0f)),
  //      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //    ),
  //    gl::perspective_projection(
  //      width, height,
  //      20,
  //      1.0f, 100.0f
  //    )
  //  },
  //  gl::camera{
  //    gl::view(
  //      math::vector_3d(5, 25, 50), math::quaternion(-30, math::vector_3d(1.0f, 0.0f, 0.0f)),
  //      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
  //    ),
  //    gl::orthogonal_projection(
  //      width, height,
  //      -20.0f, 20.0f,
  //      -20.0f, 20.0f,
  //      1.0f, 100.0f
  //    )
  //  }
  //};
  //free_cameras = {
  //  gl::free_camera{
  //    gl::free_view(
  //      math::vector_3d(5, 25, 50),
  //      math::vector_3d(-30, 0, 0)
  //    ),
  //    gl::perspective_projection(
  //      width, height,
  //      20,
  //      1.0f, 100.0f
  //    )
  //  },
  //  gl::free_camera{
  //    gl::free_view(
  //      math::vector_3d(5, 25, 50),
  //      math::vector_3d(-30, 0, 0)
  //    ),
  //    gl::orthogonal_projection(
  //      width, height,
  //      -20.0f, 20.0f,
  //      -20.0f, 20.0f,
  //      1.0f, 100.0f
  //    )
  //  }
  //};
  //current_projection = 0;
  //current_view = 0;

  engine->start();

  return (0);
}