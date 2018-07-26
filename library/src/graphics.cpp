#include <graphics.hpp>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

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
  //std::cout << "quat: " << std::endl;
  //std::cout << this->object.orientation.x << " " << this->object.orientation.y << " " << this->object.orientation.z << " " << this->object.orientation.w << " " << std::endl;
  //std::cout << std::endl;
  //std::cout << "scale: " << std::endl;
  //std::cout << this->scale[0][0] << " " << this->scale[0][1] << " " << this->scale[0][2] << " " << this->scale[0][3] << " " << std::endl;
  //std::cout << this->scale[1][0] << " " << this->scale[1][1] << " " << this->scale[1][2] << " " << this->scale[1][3] << " " << std::endl;
  //std::cout << this->scale[2][0] << " " << this->scale[2][1] << " " << this->scale[2][2] << " " << this->scale[2][3] << " " << std::endl;
  //std::cout << this->scale[3][0] << " " << this->scale[3][1] << " " << this->scale[3][2] << " " << this->scale[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
view::view(
  math::vector_3d position,
  math::quaternion orientation,
  math::vector_3d velocity,
  math::quaternion rotation)
  : object(position, orientation, velocity, rotation)
  , matrix(math::matrix_4d::View(position, position + orientation * math::vector_3d(0.0f, 0.0f, -1.0f), orientation * math::vector_3d(0.0f, 1.0f, 0.0f))) {
  //std::cout << "lookat: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
free_view::free_view(math::vector_3d position, math::vector_3d euler_angle)
  : position(position)
  , euler_angle(euler_angle)
  , matrix(math::matrix_4d::kZero) {
  math::vector_3d eye = math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(euler_angle.x, euler_angle.y, euler_angle.z);
  math::vector_3d up = math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(euler_angle.x, euler_angle.y, euler_angle.z);

  this->matrix = math::matrix_4d::View(position, position + eye, up);
}
projection::projection(
  projection_type type,
  float left,
  float right,
  float bottom,
  float top,
  float z_near,
  float z_far,
  math::matrix_4d matrix)
  : type(type)
  , left(left)
  , right(right)
  , bottom(bottom)
  , top(top)
  , z_near(z_near)
  , z_far(z_far)
  , matrix(matrix) {}
orthogonal_projection::orthogonal_projection(
  float width, float height,
  float left, float right,
  float bottom, float top,
  float z_near, float z_far)
  : projection(orthogonal, left, right, bottom, top, z_near, z_far, math::matrix_4d::OrthographicProjection(left * width / height, right * width / height, bottom, top, z_near, z_far)) {
  //std::cout << "orthogonal: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
perspective_projection::perspective_projection(
  float width, float height,
  float fovy,
  float z_near, float z_far)
  : projection(perspective, fovy, 0.0f, 0.0f, 0.0f, z_near, z_far, math::matrix_4d::PerspectiveProjection(fovy, width / height, z_near, z_far)) {
  //std::cout << "frustum: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
template<class type>
graph<type>::graph()
  : root(node{ 0u })
  , counter(1u)
  , nodes(std::vector<type>())
  , edges(std::vector<node>()) {}
template graph<model>::graph();
engine::engine(
  int *argc,
  char **argv,
  std::string title,
  unsigned width,
  unsigned height)
  : window_ids(boost::container::vector<bool>())
  , program_ids(boost::container::vector<bool>())
  , shader_ids(std::vector<std::map<shader_type, unsigned>>())
  , mesh_ids(boost::container::vector<bool>())
  , vbo_ids(std::vector<std::vector<unsigned>>())
  , current_mesh_id(0)
  , width(width)
  , height(height) {
  this->window_ids.push_back(false);
  this->program_ids.push_back(false);
  this->shader_ids.push_back(std::map<shader_type, unsigned>());
  this->mesh_ids.push_back(false);
  this->vbo_ids.push_back(std::vector<unsigned>());

  // Setup GLUT
  glutInit(argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  this->window_id = glutCreateWindow(title.c_str());
  assert(glGetError() == GL_NO_ERROR);

  // Setup GLEW
  // TODO: check why glew must initialize after creating a window
  glewExperimental = GL_TRUE;
  GLenum result = glewInit();
  assert(glGetError() == GL_NO_ERROR);

  // Setup OpenGL
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glDepthRange(0.0, 1.0);
  glClearDepth(1.0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_DEBUG_OUTPUT);
  assert(glGetError() == GL_NO_ERROR);
}

engine::~engine() {
  for (auto i = 0u; i < program_ids.size(); i++) {
    if (program_ids[i]) {
      program_ids[i] = false;
      glDeleteProgram(i);
      for (auto shader : this->shader_ids[i]) {
        glDeleteShader(shader.second);
      }
    }
  }
  for (auto i = 0u; i < mesh_ids.size(); i++) {
    if (mesh_ids[i]) {
      mesh_ids[i] = false;
      glBindVertexArray(i);
      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      for (auto vbo_id : this->vbo_ids[i]) {
        glDeleteBuffers(1, &vbo_id);
      }
      this->vbo_ids[i].clear();
      glDeleteVertexArrays(1, &i);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
  }
}

bool engine::get_left_mouse_state(int button, int state) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_UP:
      return false;
    }
    return true;
  }
  return false;
}

void model::execute(kinematic_movement movement) {
  this->object.execute(movement);
}
void view::execute(kinematic_movement movement) {
  this->object.execute(movement);
  //std::cout << "before lookat: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
  this->matrix = math::matrix_4d::View(this->object.position, this->object.position + this->object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f), this->object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f));
  //std::cout << "after lookat: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
void free_view::apply_velocity(math::vector_3d velocity) {
  this->position += velocity;

  math::vector_3d eye = math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);
  math::vector_3d up = math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);

  this->matrix = math::matrix_4d::View(this->position, this->position + eye, up);
}
void free_view::apply_rotation(math::vector_3d euler_angle) {
  this->euler_angle += euler_angle;

  math::vector_3d eye = math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);
  math::vector_3d up = math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);

  this->matrix = math::matrix_4d::View(this->position, this->position + eye, up);
}
void projection::reshape(float width, float height) {
  glViewport(0, 0, width, height);

  float ratio = width / height;
  if (this->type == perspective) {
    this->matrix = math::matrix_4d::PerspectiveProjection(
      this->left, //fovy
      ratio,
      this->z_near, this->z_far
    );
  }
  else {
    this->matrix = math::matrix_4d::OrthographicProjection(
      this->left * ratio, this->right * ratio,
      this->bottom, this->top,
      this->z_near, this->z_far
    );
  }
}

template<class type>
node* graph<type>::set_root(type content) {
  node* root = new node{ this->counter++ };
  this->nodes.push_back(content);
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
  return nodes[node.id];
}
template node* graph<model>::set_root(model);
template node* graph<model>::set_child(node, model);
template model& graph<model>::get(node);

void engine::set_close_callback(void(*handler)(void)) {
  glutCloseFunc(handler);
}
void engine::set_display_callback(void(*handler)(void)) {
  glutDisplayFunc(handler);
}
void engine::set_idle_callback(void(*handler)(void)) {
  glutIdleFunc(handler);
}
void engine::set_reshape_callback(void(*handler)(int, int)) {
  glutReshapeFunc(handler);
}
void engine::set_keyboard_callback(void(*handler)(unsigned char, int, int)) {
  glutKeyboardFunc(handler);
}
void engine::set_motion_callback(void(*handler)(int, int)) {
  glutMotionFunc(handler);
}
void engine::set_mouse_callback(void(*handler)(int, int, int, int)) {
  glutMouseFunc(handler);
}
void engine::set_timer_callback(const unsigned time, void(*handler)(int), const int value) {
  glutTimerFunc(
    time,
    handler,
    value
  );
}
void engine::set_title(std::string title_name) {
  glutSetWindow(this->window_id);
  glutSetWindowTitle(title_name.c_str());
}

constexpr GLenum to_glenum(shader_type type) {
  switch (type) {
  case compute: return (GL_COMPUTE_SHADER);
  case vertex: return (GL_VERTEX_SHADER);
  case geometry: return (GL_GEOMETRY_SHADER);
  case tess_control: return (GL_TESS_CONTROL_SHADER);
  case tess_evaluation: return (GL_TESS_EVALUATION_SHADER);
  default: return (GL_FRAGMENT_SHADER);
  }
}

void engine::start() {
  glutMainLoop();
}
void engine::display() {
  glutPostRedisplay();
}

program* engine::create_program() {
  unsigned id = glCreateProgram();
  assert(glGetError() == GL_NO_ERROR);

  this->shader_ids.insert(this->shader_ids.begin() + id, std::map<shader_type, unsigned>());
  return new program{ id };
}
template<shader_type type>
void engine::load_shader(program program, std::string filename) {
  assert(this->shader_ids.size() > program.id && this->shader_ids[program.id].find(type) == this->shader_ids[program.id].end());

  std::ifstream shader_ifstream(filename);
  std::string shader_string{
    std::istreambuf_iterator<char>{shader_ifstream},
    {}
  };
  const GLchar* shader_source = shader_string.c_str();

  auto shader_type = to_glenum(type);
  auto shader_id = glCreateShader(shader_type);
  assert(glGetError() == GL_NO_ERROR);

  glShaderSource(shader_id, 1, &shader_source, 0);
  assert(glGetError() == GL_NO_ERROR);

  glCompileShader(shader_id);
  assert(glGetError() == GL_NO_ERROR);

  GLint is_compiled = 0;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    std::cerr << "Error: Shaders";

    std::cerr << " weren't properly compiled.";
    exit(EXIT_FAILURE);
  }

  glAttachShader(program.id, shader_id);
  assert(glGetError() == GL_NO_ERROR);

  this->shader_ids[program.id].insert_or_assign(type, shader_id);
}
template void engine::load_shader<compute>(program program, std::string);
template void engine::load_shader<vertex>(program program, std::string);
template void engine::load_shader<geometry>(program program, std::string);
template void engine::load_shader<tess_control>(program program, std::string);
template void engine::load_shader<tess_evaluation>(program program, std::string);
template void engine::load_shader<fragment>(program program, std::string);
uniform* engine::get_uniform(program program, std::string name) {
  assert(this->shader_ids.size() > program.id && this->shader_ids[program.id].size() > 0);

  int id = glGetUniformLocation(program.id, name.c_str());
  assert(glGetError() == GL_NO_ERROR);

  return new uniform{ id };
}
uniform_block* engine::get_uniform_block(program program, std::string name) {
  assert(this->shader_ids.size() > program.id && this->shader_ids[program.id].size() > 0);

  unsigned id = glGetUniformBlockIndex(program.id, name.c_str());
  assert(glGetError() == GL_NO_ERROR);

  return new uniform_block{ id };
}
void engine::link(program program) {
  assert(this->shader_ids.size() > program.id && this->shader_ids[program.id].size() > 0);

  glLinkProgram(program.id);
  assert(glGetError() == GL_NO_ERROR);

  glValidateProgram(program.id);
  assert(glGetError() == GL_NO_ERROR);

  for (auto shader : this->shader_ids[program.id]) {
    glDetachShader(program.id, shader.second);
  }
  assert(glGetError() == GL_NO_ERROR);

  this->program_ids.insert(this->program_ids.begin() + program.id, true);
}

scene* engine::load_scene(program program, loader::obj obj, loader::mtl mtl, uniform_block mvp, uniform_block material) {
  assert(this->program_ids.size() > program.id && this->program_ids[program.id]);

  unsigned id;
  auto scene = new std::vector<mesh>();
  for (auto n = 0; n < obj.o.size(); ++n) {
    // generate Vertex Array for mesh
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    this->vbo_ids.push_back(std::vector<unsigned>());

    assert(glGetError() == GL_NO_ERROR);

    // buffer for vertex positions
    if (obj.f[n][0][0] > 0) {
      std::vector<float> v;
      int i;
      unsigned buffer;
      for (auto f : obj.f[n]) {
        i = f[0] - 1;
        v.push_back(obj.v[i][0]);
        v.push_back(obj.v[i][1]);
        v.push_back(obj.v[i][2]);
      }
      glGenBuffers(1, &buffer);
      this->vbo_ids[id].push_back(buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), v.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
      assert(glGetError() == GL_NO_ERROR);
    }

    // buffer for vertex normals
    if (obj.f[n][0][2] > 0) {
      std::vector<float> vn;
      int i;
      unsigned buffer;
      for (auto f : obj.f[n]) {
        i = f[2] - 1;
        vn.push_back(obj.vn[i][0]);
        vn.push_back(obj.vn[i][1]);
        vn.push_back(obj.vn[i][2]);
      }
      glGenBuffers(1, &buffer);
      this->vbo_ids[id].push_back(buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vn.size(), vn.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
      assert(glGetError() == GL_NO_ERROR);
    }

    // buffer for vertex texture coordinates
    if (obj.f[n][0][1] > 0) {
      std::vector<float> vt;
      int i;
      unsigned buffer;
      for (auto f : obj.f[n]) {
        i = f[1] - 1;
        vt.push_back(obj.vt[i][0]);
        vt.push_back(obj.vt[i][1]);
      }
      glGenBuffers(1, &buffer);
      this->vbo_ids[id].push_back(buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vt.size(), vt.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, 0);
      assert(glGetError() == GL_NO_ERROR);
    }

    // Generate uniform buffer
    unsigned mvp_id;
    glGenBuffers(1, &mvp_id);
    glBindBuffer(GL_UNIFORM_BUFFER, mvp_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 3, 0, GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0u, mvp_id);
    glUniformBlockBinding(program.id, mvp.id, GL_ZERO);
    assert(glGetError() == GL_NO_ERROR);

    // Generate uniform buffer
    unsigned material_id;
    glGenBuffers(1, &material_id);
    glBindBuffer(GL_UNIFORM_BUFFER, material_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 13 + sizeof(int), 0, GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0u, material_id);
    glUniformBlockBinding(program.id, material.id, GL_ZERO);
    assert(glGetError() == GL_NO_ERROR);

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    assert(glGetError() == GL_NO_ERROR);

    this->mesh_ids.insert(this->mesh_ids.begin() + id, true);
    std::string usemtl = obj.usemtl[n];
    if (mtl.filename.find(obj.mtllib) != std::string::npos && usemtl.compare("None") != 0) {
      for (auto i = 0u; i < mtl.newmtl.size(); i++) {
        if (mtl.newmtl[i].compare(usemtl) == 0) {
          scene->push_back(
            mesh{
              id,
              obj.f[n].size(),
              program.id,
              0,
              0,
              mvp_id,
              material_id,

              true,
              mtl.Ka[i],
              mtl.Kd[i],
              mtl.Ks[i],
              mtl.Ke[i],
              mtl.Ni[i],
              0
            }
          );
          break;
        }
      }
    }
    else {
      scene->push_back(
        mesh{
          id,
          obj.f[n].size(),
          program.id,
          0,
          0,

          false
        }
      );
    }
  }
  return scene;
}

void engine::use(program program) {
  assert(this->program_ids.size() > program.id && this->program_ids[program.id]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program.id);
  assert(glGetError() == GL_NO_ERROR);

  this->current_program_id = program.id;
}
void engine::bind(mesh scene) {
  assert(
    this->current_program_id == scene.program_id &&
    this->mesh_ids.size() > scene.id &&
    this->mesh_ids[scene.id]
  );

  glBindVertexArray(scene.id);
  this->current_mesh_id = scene.id;
}

void engine::set_uniform(uniform uniform, math::matrix_4d buffer) {
  //std::cout << "wrong final: " << std::endl;
  //std::cout << buffer[0][0] << " " << buffer[0][1] << " " << buffer[0][2] << " " << buffer[0][3] << " " << std::endl;
  //std::cout << buffer[1][0] << " " << buffer[1][1] << " " << buffer[1][2] << " " << buffer[1][3] << " " << std::endl;
  //std::cout << buffer[2][0] << " " << buffer[2][1] << " " << buffer[2][2] << " " << buffer[2][3] << " " << std::endl;
  //std::cout << buffer[3][0] << " " << buffer[3][1] << " " << buffer[3][2] << " " << buffer[3][3] << " " << std::endl;
  //std::cout << std::endl;
  glUniformMatrix4fv(uniform.id, 1, GL_FALSE, buffer.values.data());
}
//program& program::operator+=(uniform<int, 1, 1> buffer) {
//  glUniform1iv(buffer.id, 1, &buffer.data);
//  return (*this);
//}
//program& program::operator+=(uniform<int, 1, 2> buffer) {
//  glUniform2iv(buffer.id, 2, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<int, 1, 3> buffer) {
//  glUniform3iv(buffer.id, 3, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<int, 1, 4> buffer) {
//  glUniform4iv(buffer.id, 4, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<unsigned, 1, 1> buffer) {
//  glUniform1uiv(buffer.id, 1, &buffer.data);
//  return (*this);
//}
//program& program::operator+=(uniform<unsigned, 1, 2> buffer) {
//  glUniform2uiv(buffer.id, 2, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<unsigned, 1, 3> buffer) {
//  glUniform3uiv(buffer.id, 3, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<unsigned, 1, 4> buffer) {
//  glUniform4uiv(buffer.id, 4, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 1, 1> buffer) {
//  glUniform1fv(buffer.id, 1, &buffer.data);
//  return (*this);
//}
//program& program::operator+=(uniform<float, 1, 2> buffer) {
//  glUniform2fv(buffer.id, 2, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 1, 3> buffer) {
//  glUniform3fv(buffer.id, 3, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 1, 4> buffer) {
//  glUniform4fv(buffer.id, 4, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 2, 2> buffer) {
//  glUniformMatrix2fv(buffer.id, 4, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 2, 3> buffer) {
//  glUniformMatrix2x3fv(buffer.id, 6, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 2, 4> buffer) {
//  glUniformMatrix2x4fv(buffer.id, 8, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 3, 2> buffer) {
//  glUniformMatrix3x2fv(buffer.id, 6, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 3, 3> buffer) {
//  glUniformMatrix3fv(buffer.id, 9, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 3, 4> buffer) {
//  glUniformMatrix3x4fv(buffer.id, 12, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 4, 2> buffer) {
//  glUniformMatrix4x2fv(buffer.id, 8, GL_FALSE, buffer.data.data());
//  return (*this);
//}
//program& program::operator+=(uniform<float, 4, 3> buffer) {
//  glUniformMatrix4x3fv(buffer.id, 12, GL_FALSE, buffer.data.data());
//  return (*this);
//}
void engine::draw(mesh scene) {
  assert(this->current_mesh_id == scene.id);

  glDrawElements(GL_TRIANGLES, scene.n_faces, GL_UNSIGNED_INT, NULL);
}
void engine::draw(
  graph<model> scene_graph,
  camera camera) {
  this->draw(
    std::cref(scene_graph),
    std::cref(camera.projection.matrix),
    std::cref(camera.view.matrix)
  );
}
void engine::draw(
  graph<model> scene_graph,
  free_camera camera) {
  this->draw(
    std::cref(scene_graph),
    std::cref(camera.projection.matrix),
    std::cref(camera.view.matrix)
  );
}
void engine::draw(
  graph<model> scene_graph,
  math::matrix_4d projection,
  math::matrix_4d view
) {
  std::vector<math::matrix_4d> translations;
  std::vector<math::matrix_4d> rotations;
  std::vector<math::matrix_4d> scalations;

  math::matrix_4d translation = math::matrix_4d::kIdentity;
  math::matrix_4d rotation = math::matrix_4d::kIdentity;
  math::matrix_4d scalation = math::matrix_4d::kIdentity;

  translations.push_back(math::matrix_4d::kIdentity);
  rotations.push_back(math::matrix_4d::kIdentity);
  scalations.push_back(math::matrix_4d::kIdentity);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto i = 0u; i < scene_graph.nodes.size(); i++) {
    auto model = scene_graph.nodes[i];
    auto parent_id = scene_graph.edges[i].id;

    math::matrix_4d buffer = math::matrix_4d(model.object.orientation);
    translation = math::matrix_3d::Translation(model.object.position.x, model.object.position.y, model.object.position.z) * translations[parent_id];
    rotation = math::matrix_4d(model.object.orientation) * rotations[parent_id];
    scalation = model.scale * scalations[parent_id];

    //math::matrix_4d buffer = math::matrix_4d(model.object.orientation);
    //std::cout << "rotation: " << std::endl;
    //std::cout << buffer[0][0] << " " << buffer[0][1] << " " << buffer[0][2] << " " << buffer[0][3] << " " << std::endl;
    //std::cout << buffer[1][0] << " " << buffer[1][1] << " " << buffer[1][2] << " " << buffer[1][3] << " " << std::endl;
    //std::cout << buffer[2][0] << " " << buffer[2][1] << " " << buffer[2][2] << " " << buffer[2][3] << " " << std::endl;
    //std::cout << buffer[3][0] << " " << buffer[3][1] << " " << buffer[3][2] << " " << buffer[3][3] << " " << std::endl;
    //std::cout << std::endl;

    //buffer = math::matrix_3d::Translation(model.object.position.x, model.object.position.y, model.object.position.z);
    //std::cout << "translation: " << std::endl;
    //std::cout << buffer[0][0] << " " << buffer[0][1] << " " << buffer[0][2] << " " << buffer[0][3] << " " << std::endl;
    //std::cout << buffer[1][0] << " " << buffer[1][1] << " " << buffer[1][2] << " " << buffer[1][3] << " " << std::endl;
    //std::cout << buffer[2][0] << " " << buffer[2][1] << " " << buffer[2][2] << " " << buffer[2][3] << " " << std::endl;
    //std::cout << buffer[3][0] << " " << buffer[3][1] << " " << buffer[3][2] << " " << buffer[3][3] << " " << std::endl;
    //std::cout << std::endl;

    //buffer = math::matrix_3d::Translation(model.object.position.x, model.object.position.y, model.object.position.z) * translations[parent_id];
    //std::cout << "multiplication: " << std::endl;
    //std::cout << buffer[0][0] << " " << buffer[0][1] << " " << buffer[0][2] << " " << buffer[0][3] << " " << std::endl;
    //std::cout << buffer[1][0] << " " << buffer[1][1] << " " << buffer[1][2] << " " << buffer[1][3] << " " << std::endl;
    //std::cout << buffer[2][0] << " " << buffer[2][1] << " " << buffer[2][2] << " " << buffer[2][3] << " " << std::endl;
    //std::cout << buffer[3][0] << " " << buffer[3][1] << " " << buffer[3][2] << " " << buffer[3][3] << " " << std::endl;
    //std::cout << std::endl;

    for (auto mesh : model.scene_info) {
      if (this->current_program_id != mesh.program_id) {
        assert(
          this->program_ids.size() > mesh.program_id &&
          this->program_ids[mesh.program_id]
        );

        this->current_program_id = mesh.program_id;
        glUseProgram(current_program_id);
        assert(glGetError() == GL_NO_ERROR);
      }
      if (this->current_mesh_id != mesh.id) {
        assert(
          this->mesh_ids.size() > mesh.id &&
          this->mesh_ids[mesh.id]
        );

        this->current_mesh_id = mesh.id;
        glBindVertexArray(current_mesh_id);
        assert(glGetError() == GL_NO_ERROR);
      }

      //this->set_uniform(
      //  uniform,
      //  projection *
      //  view *
      //  translation *
      //  rotation *
      //  scalation
      //);

      glBindBuffer(GL_UNIFORM_BUFFER, mesh.mvp_id);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 16, projection.values.data());
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16, sizeof(float) * 16, view.values.data());
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 32, sizeof(float) * 16, (translation * rotation * scalation).values.data());
      glBindBuffer(GL_UNIFORM_BUFFER, mesh.material_id);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 3, mesh.ambient.data());
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 3, sizeof(float) * 3, mesh.diffuse.data());
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 6, sizeof(float) * 3, mesh.specular.data());
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 9, sizeof(float) * 3, mesh.emissive.data());
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 12, sizeof(float), &mesh.shininess);
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 13, sizeof(int), &mesh.n_textures);
      glBindBuffer(GL_UNIFORM_BUFFER, GL_ZERO);
      assert(glGetError() == GL_NO_ERROR);

      glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.n_faces * 3);
      assert(glGetError() == GL_NO_ERROR);
    }

    translations.push_back(translation);
    rotations.push_back(rotation);
    scalations.push_back(scalation);
  }

  glutSwapBuffers();
}

void engine::unbind() {
  glBindVertexArray(0);

  this->current_mesh_id = 0;
}
void engine::unuse() {
  glutSwapBuffers();

  this->current_program_id = 0;
}
