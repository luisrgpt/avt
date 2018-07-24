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
gl::scene *scene;
gl::uniform *projection_view_model;
gl::graph<gl::model> *scene_graph;
std::vector<gl::camera> cameras;
unsigned current_camera;

/////////////////////////////////////////////////////////////////////// CALLBACKS

void close() {}
void idle() {}
void keyboard(unsigned char key, int x, int y) {
//    math::vector_3d current_position = state_position;
    switch (key) {
      case '1':
        current_camera = 0;
        break;
      case '2':
        current_camera = 1;
        break;
      //case '3':
      //  current_camera = 2;
      //  break;
      case 'a':
        for (auto &camera : cameras) {
          camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(-1.0f, 0.0f, 0.0f), 1));
        }
        break;
      case 'd':
        for (auto &camera : cameras) {
          camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f), 1));
        }
        break;
      case 's':
        for (auto &camera : cameras) {
          camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, -1.0f, 0.0f), 1));
        }
        break;
      case 'w':
        for (auto &camera : cameras) {
          camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f), 1));
        }
        break;
      case 'q':
        for (auto &camera : cameras) {
          camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f), 1));
        }
        break;
      case 'e':
        for (auto &camera : cameras) {
          camera.view.execute(gl::linear_movement(camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f), 1));
        }
        break;
      case 'j':
        for (auto &camera : cameras) {
          camera.view.execute(gl::rotation(math::quaternion(0.02f, camera.view.object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
        }
        break;
      case 'l':
        for (auto &camera : cameras) {
          camera.view.execute(gl::rotation(math::quaternion(0.02f, camera.view.object.orientation * math::vector_3d(0.0f, -1.0f, 0.0f)), 1));
        }
        break;
      case 'k':
        for (auto &camera : cameras) {
          camera.view.execute(gl::rotation(math::quaternion(0.02f, camera.view.object.orientation * math::vector_3d(-1.0f, 0.0f, 0.0f)), 1));
        }
        break;
      case 'i':
        for (auto &camera : cameras) {
          camera.view.execute(gl::rotation(math::quaternion(0.02f, camera.view.object.orientation * math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
        }
        break;
      case 'u':
        for (auto &camera : cameras) {
          camera.view.execute(gl::rotation(math::quaternion(0.02f, camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f)), 1));
        }
        break;
      case 'o':
        for (auto &camera : cameras) {
          camera.view.execute(gl::rotation(math::quaternion(0.02f, camera.view.object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f)), 1));
        }
        break;
      case 'x':
        delete engine;
        break;
    }
}
void motion(int x, int y) {
//    if (state_mouse_motion) {
//      std::cout << (x - state_x) / ((float)window.width) << ":" << (y - state_y) / ((float)window.height) << std::endl;
//
//      math::vector_3d state_right = glm::normalize(glm::rotate(state_right, (y - state_y) / ((float) window.height), state_up));
//      math::vector_3d state_front = glm::normalize(glm::rotate(glm::normalize(glm::rotate(state_front, (x - state_x) / ((float)window.width), state_up)), state_eye_y, state_right));
//      math::vector_3d state_up = glm::normalize(glm::cross(state_right, state_front));
//
//      view = glm::lookAt(state_position, state_position + state_front, state_up);
//    }
//
//    state_x = x;
//    state_y = y;
//    state_mouse_motion = true;
}
void mouse(int button, int state, int x, int y) {
//    switch (button) {
//      case GLUT_LEFT_BUTTON:
//        switch (state) {
//          case GLUT_UP:
//            state_mouse_motion = false;
//            break;
//        }
//        state_mouse_button = state;
//        break;
//    }
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

  engine->draw(
    *projection_view_model,
    *scene_graph,
    cameras[current_camera]
  );
  //std::cout << std::endl;
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
  engine->set_timer_callback(mhz , timer, 0);

  engine->display();
}
void reshape(int width, int height) {
  cameras[current_camera].projection.reshape(width, height);
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
  engine->set_timer_callback(0 , timer, 0);

  program = engine->create_program();
  engine->load_shader<gl::vertex>(*program, "share/position_vertex.glsl");
  engine->load_shader<gl::fragment>(*program, "share/position_fragment.glsl");
  engine->link(*program);
  projection_view_model = engine->get_uniform(*program, "projection_view_model");

  scene = engine->load_scene<gl::obj>(*program, "share/cube.obj");

  scene_graph = new gl::graph<gl::model>();
  auto front = scene_graph->set_root(
    gl::model(
      *scene,
      math::vector_3d(10, 10, 10),
      math::vector_3d(0, 0, 0), math::quaternion(0.2f, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  scene_graph->set_root(
    gl::model(
      *scene,
      math::vector_3d(10, 10, 10),
      math::vector_3d(20, 0, 20), math::quaternion(0.0f, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  scene_graph->set_child(
    *front,
    gl::model(
      *scene,
      math::vector_3d(1, 1, 1),
      math::vector_3d(0, 10, 0), math::quaternion(0.2f, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  cameras = {
    gl::camera{
    gl::view(
      math::vector_3d(0, 0, 50), math::quaternion(0.0f, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    ),
    gl::perspective_projection(
      width, height,
      -0.3f, 0.3f,
      -0.3f, 0.3f,
      1.0f, 100.0f
    )
  },
    gl::camera{
    gl::view(
      math::vector_3d(0, 0, 50), math::quaternion(0.0f, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    ),
    gl::orthogonal_projection(
      width, height,
      -10.0f, 10.0f,
      -10.0f, 10.0f,
      1.0f, 100.0f
    )
  }
  };
  current_camera = 0;

  engine->start();

  return (0);
}