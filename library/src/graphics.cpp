#include <gl/graphics.hpp>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace gl;

model::model(
  mesh mesh,
  glm::vec3 scale,
  glm::vec3 position,
  glm::quat orientation,
  glm::vec3 velocity,
  glm::quat rotation)
  : mesh_info(mesh)
  , object(position, orientation, velocity, rotation)
  , scale(glm::scale(scale)) {}
view::view(
  glm::vec3 position,
  glm::quat orientation,
  glm::vec3 velocity,
  glm::quat rotation)
  : object(position, orientation, velocity, rotation)
  , matrix(glm::lookAt(position, orientation * glm::vec3(0.0f, 0.0f, 1.0f), orientation * glm::vec3(0.0f, 1.0f, 0.0f))) {}
projection::projection(
  projection_type type,
  float left,
  float right,
  float bottom,
  float top,
  float z_near,
  float z_far,
  glm::mat4 matrix)
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
  : projection(orthogonal, left, right, bottom, top, z_near, z_far, glm::ortho(left * width/height, right * width/height, bottom, top, z_near, z_far)) {}
perspective_projection::perspective_projection(
  float width, float height,
  float left, float right,
  float bottom, float top,
  float z_near, float z_far)
  : projection(perspective, left, right, bottom, top, z_near, z_far, glm::frustum(left * width/height, right * width/height, bottom, top, z_near, z_far)) {}
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
  , current_mesh_id(0)
  , width(width)
  , height(height) {
  this->window_ids.push_back(false);
  this->program_ids.push_back(false);
  this->shader_ids.push_back(std::map<shader_type, unsigned>());
  this->mesh_ids.push_back(false);

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

void model::execute(kinematic_movement movement) {
  this->object.execute(movement);
}
void view::execute(kinematic_movement movement) {
  this->object.execute(movement);
  this->matrix = glm::lookAt(this->object.position, this->object.position + this->object.orientation * glm::vec3(0.0f, 0.0f, -1.0f), this->object.orientation * glm::vec3(0.0f, 1.0f, 0.0f));
}
void projection::reshape(float width, float height) {
  glViewport(0, 0, width, height);

  float ratio = width / height;
  if (this->type == perspective) {
    this->matrix = glm::frustum(
      this->left * ratio, this->right * ratio,
      this->bottom, this->top,
      this->z_near, this->z_far
    );
  }
  else {
    this->matrix = glm::ortho(
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

mesh* engine::load_mesh(program program, std::string filename) {
  assert(this->program_ids.size() > program.id && this->program_ids[program.id]);

  Assimp::Importer importer;
  auto *scene = importer.ReadFile(filename, NULL);
  //for (auto i = 0u; i < scene->mNumMeshes; ++i) {
  auto *ai_mesh = scene->mMeshes[0];

  // create array with faces
  // have to convert from Assimp format to array
  unsigned buffer;
  unsigned int *faceArray;
  faceArray = (unsigned int *)malloc(sizeof(unsigned int) * ai_mesh->mNumFaces * 3);
  unsigned int faceIndex = 0;

  for (unsigned int t = 0; t < ai_mesh->mNumFaces; ++t) {
    const aiFace* face = &ai_mesh->mFaces[t];

    memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
    faceIndex += 3;
  }

  // generate Vertex Array for ai_mesh
  unsigned id;
  glCreateVertexArrays(1, &id);
  glBindVertexArray(id);

  // buffer for faces
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ai_mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

  // buffer for vertex positions
  if (ai_mesh->HasPositions()) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * ai_mesh->mNumVertices, ai_mesh->mVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
  }

  // buffer for vertex normals
  //if (ai_mesh->HasNormals()) {
  //  glGenBuffers(1, &buffer);
  //  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * ai_mesh->mNumVertices, ai_mesh->mNormals, GL_STATIC_DRAW);
  //  glEnableVertexAttribArray(1);
  //  glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
  //}

  // buffer for vertex texture coordinates
  //if (ai_mesh->HasTextureCoords(0)) {
  //  float *texCoords = (float *)malloc(sizeof(float) * 2 * ai_mesh->mNumVertices);
  //  for (unsigned int k = 0; k < ai_mesh->mNumVertices; ++k) {

  //    texCoords[k * 2] = ai_mesh->mTextureCoords[0][k].x;
  //    texCoords[k * 2 + 1] = ai_mesh->mTextureCoords[0][k].y;

  //  }
  //  glGenBuffers(1, &buffer);
  //  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * ai_mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
  //  glEnableVertexAttribArray(2);
  //  glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, 0);
  //}

  // unbind buffers
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  this->mesh_ids.insert(this->mesh_ids.begin() + id, true);

  return new mesh{ id, ai_mesh->mNumFaces * 3, program.id };
}

void engine::use(program program) {
  assert(this->program_ids.size() > program.id && this->program_ids[program.id]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program.id);
  assert(glGetError() == GL_NO_ERROR);

  this->current_program_id = program.id;
}
void engine::bind(mesh mesh) {
  assert(
    this->current_program_id == mesh.program_id &&
    this->mesh_ids.size() > mesh.id &&
    this->mesh_ids[mesh.id]
  );

  glBindVertexArray(mesh.id);
  this->current_mesh_id = mesh.id;
}

void engine::set_uniform(uniform uniform, glm::mat4 buffer) {
  glUniformMatrix4fv(uniform.id, 1, GL_FALSE, glm::value_ptr(buffer));
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
void engine::draw(mesh mesh) {
  assert(this->current_mesh_id == mesh.id);

  glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, NULL);
}
void engine::draw(
  uniform uniform,
  graph<model> scene_graph,
  camera camera
) {
  std::vector<glm::mat4> translations;
  std::vector<glm::mat4> rotations;
  std::vector<glm::mat4> scalations;

  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 translation;
  glm::mat4 rotation;
  glm::mat4 scalation;

  translations.push_back(glm::mat4(1.0f));
  rotations.push_back(glm::mat4(1.0f));
  scalations.push_back(glm::mat4(1.0f));

  projection = camera.projection.matrix;
  view = camera.view.matrix;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto i = 0u; i < scene_graph.nodes.size(); i++) {
    auto model = scene_graph.nodes[i];
    auto parent_id = scene_graph.edges[i].id;

    translation = glm::translate(model.object.position) * translations[parent_id];
    rotation = glm::mat4_cast(model.object.orientation) * rotations[parent_id];
    scalation = model.scale * scalations[parent_id];

    if (this->current_program_id != model.mesh_info.program_id) {
      assert(
        this->program_ids.size() > model.mesh_info.program_id &&
        this->program_ids[model.mesh_info.program_id]
      );

      this->current_program_id = model.mesh_info.program_id;
      glUseProgram(current_program_id);
      assert(glGetError() == GL_NO_ERROR);
    }
    if (this->current_mesh_id != model.mesh_info.id) {
      assert(
        this->mesh_ids.size() > model.mesh_info.id &&
        this->mesh_ids[model.mesh_info.id]
      );

      this->current_mesh_id = model.mesh_info.id;
      glBindVertexArray(current_mesh_id);
      assert(glGetError() == GL_NO_ERROR);
    }

    this->set_uniform(
      uniform,
      projection *
      view *
      translation *
      rotation *
      scalation
    );

    glDrawElements(GL_TRIANGLES, model.mesh_info.size, GL_UNSIGNED_INT, NULL);
    assert(glGetError() == GL_NO_ERROR);

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
