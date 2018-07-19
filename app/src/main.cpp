#include <gl/graphics.hpp>
#include <iostream>

constexpr unsigned
  fps = 60,
  mhz = 1000 / fps,
  window_width = 640,
  window_height = 480;
constexpr float
  projection_width = window_width / 2.0f,
  projection_height = window_height / 2.0f;
gl::engine *engine;
gl::program *program;
gl::window *window;
std::vector<gl::model> models;
std::vector<gl::camera> cameras;
unsigned current_camera;

/////////////////////////////////////////////////////////////////////// CALLBACKS

void close() {}
void idle() {}
void keyboard(unsigned char key, int x, int y) {
//    glm::vec3 current_position = state_position;
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
        cameras[1].view += gl::linear_movement(glm::vec3(-100.0f, 0.0f, 0.0f), 1);
        break;
      case 'd':
        cameras[1].view += gl::linear_movement(glm::vec3(100.0f, 0.0f, 0.0f), 1);
        break;
//        is_perspective = !is_perspective;
//        projection = kProjections[is_perspective];
//        break;
//      case 'w':
//        state_position = current_position + state_front * 0.1f;
//
//        view = glm::lookAt(state_position, state_position + state_front, state_up);
//        return;
//      case 's':
//        state_position = current_position - state_front * 0.1f;
//
//        view = glm::lookAt(state_position, state_position + state_front, state_up);
//        return;
    }
}
void motion(int x, int y) {
//    if (state_mouse_motion) {
//      std::cout << (x - state_x) / ((float)window.width) << ":" << (y - state_y) / ((float)window.height) << std::endl;
//
//      glm::vec3 state_right = glm::normalize(glm::rotate(state_right, (y - state_y) / ((float) window.height), state_up));
//      glm::vec3 state_front = glm::normalize(glm::rotate(glm::normalize(glm::rotate(state_front, (x - state_x) / ((float)window.width), state_up)), state_eye_y, state_right));
//      glm::vec3 state_up = glm::normalize(glm::cross(state_right, state_front));
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
  *engine += *program;
  for (gl::model& model : models) {
    *engine += model;

    //std::cout << "Model:" << std::endl;
    //std::cout << model.matrix[0][0] << " " << model.matrix[0][1] << " " << model.matrix[0][2] << " " << model.matrix[0][3] << std::endl;
    //std::cout << model.matrix[1][0] << " " << model.matrix[1][1] << " " << model.matrix[1][2] << " " << model.matrix[1][3] << std::endl;
    //std::cout << model.matrix[2][0] << " " << model.matrix[2][1] << " " << model.matrix[2][2] << " " << model.matrix[2][3] << std::endl;
    //std::cout << model.matrix[3][0] << " " << model.matrix[3][1] << " " << model.matrix[3][2] << " " << model.matrix[3][3] << std::endl;
    //std::cout << std::endl;
    //std::cout << "View:" << std::endl;
    //std::cout << view->matrix[0][0] << " " << view->matrix[0][1] << " " << view->matrix[0][2] << " " << view->matrix[0][3] << std::endl;
    //std::cout << view->matrix[1][0] << " " << view->matrix[1][1] << " " << view->matrix[1][2] << " " << view->matrix[1][3] << std::endl;
    //std::cout << view->matrix[2][0] << " " << view->matrix[2][1] << " " << view->matrix[2][2] << " " << view->matrix[2][3] << std::endl;
    //std::cout << view->matrix[3][0] << " " << view->matrix[3][1] << " " << view->matrix[3][2] << " " << view->matrix[3][3] << std::endl;
    //std::cout << std::endl;
    //std::cout << "Projection:" << std::endl;
    //std::cout << cameras[current_camera].projection.matrix[0][0] << " " << cameras[current_camera].projection.matrix[0][1] << " " << cameras[current_camera].projection.matrix[0][2] << " " << cameras[current_camera].projection.matrix[0][3] << std::endl;
    //std::cout << cameras[current_camera].projection.matrix[1][0] << " " << cameras[current_camera].projection.matrix[1][1] << " " << cameras[current_camera].projection.matrix[1][2] << " " << cameras[current_camera].projection.matrix[1][3] << std::endl;
    //std::cout << cameras[current_camera].projection.matrix[2][0] << " " << cameras[current_camera].projection.matrix[2][1] << " " << cameras[current_camera].projection.matrix[2][2] << " " << cameras[current_camera].projection.matrix[2][3] << std::endl;
    //std::cout << cameras[current_camera].projection.matrix[3][0] << " " << cameras[current_camera].projection.matrix[3][1] << " " << cameras[current_camera].projection.matrix[3][2] << " " << cameras[current_camera].projection.matrix[3][3] << std::endl;
    //std::cout << std::endl;

    *engine += (cameras[current_camera].projection.matrix * (cameras[current_camera].view.matrix * model.matrix));
    *engine -= model;
  }
  *engine -= *program;
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

  *window += gl::title{ title.c_str() };
  *window += gl::callback::timer{ mhz , timer, 0 };

  *engine += *window;
}
void reshape(int width, int height) {
  window->width = width;
  window->height = height;

  cameras[current_camera].projection += *window;
}

int main(int argc, char **argv) {
  engine = new gl::engine();
  window = new gl::window(&argc, argv, "avt", window_width, window_height);
  program = new gl::program();
  models = {
    gl::model(
      "share/cube.ply",
      glm::vec3(600, 600, 600),
      glm::vec3(0, 0, 0), glm::quat(0, 0, 0, 0),
      glm::vec3(0, 0, 0), glm::quat(0, 0, 0, 0)
    )
  };
  cameras = {
    gl::camera{
      gl::view(
        glm::vec3(0, 0, 610), glm::quat(0, 0, 0, 0),
        glm::vec3(0, 0, 0), glm::quat(0, 0, 0, 0)
      ),
      gl::projection(
        gl::perspective,
        -projection_width, projection_width,
        -projection_height, projection_height,
         10.0f, 1000.0f
      )
    },
    gl::camera{
    gl::view(
      glm::vec3(0, 0, 610), glm::quat(0, 0, 0, 0),
      glm::vec3(0, 0, 0), glm::quat(0, 0, 0, 0)
    ),
    gl::projection(
      gl::orthogonal,
      -projection_width, projection_width,
      -projection_height, projection_height,
      10.0f, 1000.0f
    )
  }
  };
  current_camera = 0;

  *window += gl::callback::close{ close };
  *window += gl::callback::display{ display };
  *window += gl::callback::idle{ idle };
  *window += gl::callback::reshape{ reshape };
  *window += gl::callback::keyboard{ keyboard };
  *window += gl::callback::motion{ motion };
  *window += gl::callback::mouse{ mouse };
  *window += gl::callback::timer{0 , timer, 0};

  *program += gl::shader<gl::vertex>{ "share/position_vertex.glsl" };
  *program += gl::shader<gl::fragment>{ "share/position_fragment.glsl" };

  *engine += *program;
  for (gl::model& model : models) {
    *engine += model;
  }
  *engine += *window;

  return (0);
}