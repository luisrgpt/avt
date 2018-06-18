#include <glm/glm.hpp>
#include <gl/core.hpp>

//  const std::array<glm::mat4, 2> kProjections {
//    glm::ortho(-2, 2, -2, 2, 1, 10),
//    glm::perspective(kPi6, window.width / (float) window.height, 1.0f, 10.0f)
//  };
//
//  const std::array<glm::mat4, 7> kTransformations = {
//    glm::translate(glm::mat4(), glm::vec3(-0.5f , 0.0f, -1.5f )) * glm::rotate(glm::mat4(), 45.0f , glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(kS24, 1.0f, kS24)),
//    glm::translate(glm::mat4(), glm::vec3( 0.5f , 0.0f,  0.5f )) * glm::rotate(glm::mat4(), 0.0f  , glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(kS22, 1.0f, kS22)),
//    glm::translate(glm::mat4(), glm::vec3( 0.15f, 0.0f, -0.15f)) * glm::rotate(glm::mat4(), 0.0f  , glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(kS22, 1.0f, kS22)),
//    glm::translate(glm::mat4(), glm::vec3(-0.5f , 0.0f,  0.5f )) * glm::rotate(glm::mat4(), 45.0f , glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(0.5f, 1.0f, 0.5f)),
//    glm::translate(glm::mat4(), glm::vec3( 0.5f , 0.0f,  1.5f )) * glm::rotate(glm::mat4(), 90.0f , glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(kS24, 1.0f, kS24)),
//    glm::translate(glm::mat4(), glm::vec3(-0.5f , 0.0f,  0.5f )) * glm::rotate(glm::mat4(), 180.0f, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(kS24, 1.0f, kS24)),
//    glm::translate(glm::mat4(), glm::vec3(-1.0f , 0.0f, -0.5f )) * glm::rotate(glm::mat4(), 45.0f , glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(kS24, 1.0f, kS24))
//  };
//
//  glm::vec3 state_right = glm::normalize(glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f));
//  glm::vec3 state_front = glm::normalize(glm::rotate(glm::normalize(glm::rotateY(glm::vec3(0.0f, 0.0f, -1.0f), 0.0f)), state_eye_y, state_right));
//  glm::vec3 state_up = glm::normalize(glm::cross(state_right, state_front));
//  glm::vec3 state_position = glm::vec3(0.0f, 0.0f, 5.0f);
//
//  bool is_perspective = false;
//
//  glm::mat4 view = glm::lookAt(state_position, state_position + state_front, state_up);
//  glm::mat4 projection = kProjections[is_perspective];
//
//
//  bool state_mouse_motion;
//  int state_x = 0;
//  int state_y = 0;
//  float state_eye_x = 0.0f;
//  float state_eye_y = 0.0f;

/////////////////////////////////////////////////////////////////////// SHADERs

//EventManager(const Shader& shader, const std::vector<Shape>& shapes) :
//  _shader(shader),
//  _program(shader.CreateProgram()),
//  _vertex_arrays{
//    _program.CreateVertexArray(shapes[0]),
//    _program.CreateVertexArray(shapes[1]),
//    _program.CreateVertexArray(shapes[2]),
//    _program.CreateVertexArray(shapes[3]),
//    _program.CreateVertexArray(shapes[4]),
//    _program.CreateVertexArray(shapes[5]),
//    _program.CreateVertexArray(shapes[6])
//  } {
//}
//float state_center = 0;
//int state_mouse_button;

constexpr unsigned
  fps = 60,
  mhz = 1000 / fps;
gl::engine *engine;
gl::program *program;
gl::window *window;
std::vector<gl::model> models;
gl::view *view;
gl::projection *projection;

/////////////////////////////////////////////////////////////////////// CALLBACKS

void close() {}
void idle() {}

void keyboard(unsigned char key, int x, int y) {
//    glm::vec3 current_position = state_position;
//    switch (key) {
//      case 'p':
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
//    }
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
    *engine += (projection->matrix * (view->matrix * model.matrix));
    *engine -= model;
  }
  *engine -= *program;
//    for (auto index = 0u; index < _vertex_arrays.size(); index++) {
//      PreparedProgram prepared_program = _vertex_arrays[index].PrepareProgram();
//      prepared_program.SetUniform("ModelMatrix", 1, kTransformations[index]);
//      glBindBuffer(GL_UNIFORM_BUFFER, _vertex_arrays[index].BufferObjectLocation(2));
//      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 16, view.matrix.data());
//      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16, sizeof(float) * 16, projection.matrix.data());
//      glBindBuffer(GL_UNIFORM_BUFFER, GL_ZERO);
//      prepared_program.ExecuteProgram(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
//    }
}

void timer(int value) {
  std::string title
    = "avt -> ";
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

  *projection += *window;
}

int main() {
  engine = new gl::engine();
  program = new gl::program();
  window = new gl::window();
  models = {
    gl::model("example.ply")
  };
  view = new gl::view();
  projection = new gl::projection(
    90.0f,
    (float)window->width / (float)window->height,
    0.1f,
    10.0f
  );

  *window += gl::callback::close{ close };
  *window += gl::callback::display{ display };
  *window += gl::callback::idle{ idle };
  *window += gl::callback::reshape{ reshape };
  *window += gl::callback::keyboard{ keyboard };
  *window += gl::callback::motion{ motion };
  *window += gl::callback::mouse{ mouse };
  *window += gl::callback::timer{0 , timer, 0};

  *program += gl::shader<gl::vertex>{ "vertex.glsl" };
  *program += gl::shader<gl::fragment>{ "fragment.glsl" };

  *engine += *program;
  for (gl::model& model : models) {
    *engine += model;
  }
  *engine += *window;

  return (0);
}