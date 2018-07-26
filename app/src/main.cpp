#include <graphics.hpp>
#include <math/quaternion.hpp>
#include <math/vector_3d.hpp>
#include <iostream>
#include <functional>

constexpr unsigned
fps = 60,
mhz = 1000 / fps,
width = 640,
height = 480;

gl::engine *engine;
gl::program *program;
gl::uniform_block *projection_view_model;
gl::uniform_block *material;
gl::graph<gl::model> *scene_graph;
std::vector<gl::camera> cameras;
std::vector<gl::free_camera> free_cameras;
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
bool left_mouse_state;
int x_state = 0;
int y_state = 0;
void motion(int x, int y) {
  if (left_mouse_state) {
    std::cout << (x - x_state) / (float)fps << " " << (y - y_state) / (float)fps << std::endl;

    for (auto &camera : cameras) {
    camera.view.execute(gl::rotation(math::quaternion((x - x_state) * 10 / (float)fps, camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d(0.0f, (x - x_state) * 10 / (float)fps, 0.0f));
    }
    for (auto &camera : cameras) {
      camera.view.execute(gl::rotation(math::quaternion((y - y_state) * 10 / (float)fps, camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
    }
    for (auto &camera : free_cameras) {
      camera.view.apply_rotation(math::vector_3d((y - y_state) * 10 / (float)fps, 0.0f, 0.0f));
    }

    x_state = x;
    y_state = y;
  }
}
void mouse(int button, int state, int x, int y) {
  left_mouse_state = engine->get_left_mouse_state(button, state);
  if (left_mouse_state) {
    x_state = x;
    y_state = y;
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
  engine->load_shader<gl::vertex>(*program, "share/texture_vertex.glsl");
  engine->load_shader<gl::fragment>(*program, "share/texture_fragment.glsl");
  engine->link(*program);
  projection_view_model = engine->get_uniform_block(*program, "mvp");
  material = engine->get_uniform_block(*program, "material");

  loader::mtl materials("share/materials.mtl");
  loader::obj obj_field("share/field.obj");
  loader::obj obj_ship("share/ship.obj");
  loader::obj obj_ball("share/ball.obj");
  loader::obj obj_box("share/box.obj");
  loader::obj obj_pipe("share/big_pipe.obj");
  loader::obj obj_border("share/border.obj");

  gl::scene *field = engine->load_scene(*program, obj_field, materials, *projection_view_model, *material);
  gl::scene *ship = engine->load_scene(*program, obj_ship, materials, *projection_view_model, *material);
  gl::scene *ball = engine->load_scene(*program, obj_ball, materials, *projection_view_model, *material);
  gl::scene *box = engine->load_scene(*program, obj_box, materials, *projection_view_model, *material);
  gl::scene *big_pipe = engine->load_scene(*program, obj_pipe, materials, *projection_view_model, *material);
  gl::scene *border = engine->load_scene(*program, obj_border, materials, *projection_view_model, *material);

  scene_graph = new gl::graph<gl::model>();
  scene_graph->set_root(
    gl::model(
      *field,
      math::vector_3d(1, 1, 1),
      math::vector_3d(13.5f, 14.5f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  scene_graph->set_root(
    gl::model(
      *ship,
      math::vector_3d(1, 1, 1),
      math::vector_3d(13.5f, 2.5f, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  scene_graph->set_root(
    gl::model(
      *ball,
      math::vector_3d(1, 1, 1),
      math::vector_3d(13.5f, 3.5f, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  for (auto i = 0u; i < 6u; i++) {
    for (auto j = 0u; j < 13u; j++) {
      scene_graph->set_root(
        gl::model(
          *box,
          math::vector_3d(1, 1, 1),
          math::vector_3d(2.0f + 1.9f * j, 18.45f + 0.85f * i, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        )
      );
    }
  }
  scene_graph->set_root(
    gl::model(
      *border,
      math::vector_3d(1, 1, 1),
      math::vector_3d(13.5f, 14.3f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  for (auto i = 0u; i < 6u; i++) {
    for (auto j = 0u; j < 2u; j++) {
      scene_graph->set_root(
        gl::model(
          *big_pipe,
          math::vector_3d(1, 1, 1),
          math::vector_3d(0.5f + 26.0f * j, 1.875f + 5.0f * i, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        )
      );
    }
  }
  for (auto j = 0u; j < 2u; j++) {
    scene_graph->set_root(
      gl::model(
        *big_pipe,
        math::vector_3d(1, 1, 1),
        math::vector_3d(6.75f + 13.5f * j, 29.0f, 0.0f), math::quaternion(90, math::vector_3d(0.0f, 0.0f, 1.0f)),
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