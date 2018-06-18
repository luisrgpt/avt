#include <gl/core.hpp>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace gl;

window::window()
  : id(glutCreateWindow("avt"))
  , width(640)
  , height(480) {}
program::program()
  : id(glCreateProgram())
  , shader_ids(std::vector<unsigned>()) {}
engine::engine()
  : window_ids(std::vector<bool>())
  , program_ids(std::vector<bool>())
  , model_ids(std::vector<bool>())
  , id_counter(0) {
  // Setup OpenGL
  //std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glDepthRange(0.0, 1.0);
  glClearDepth(1.0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // Setup GLEW
  glewExperimental = GL_TRUE;
  GLenum result = glewInit();
  if (result != GLEW_OK) {
    //std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
    exit(EXIT_FAILURE);
  }
  GLenum err_code = glGetError();

  // Setup GLUT
  glutInit(0, NULL);

  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glutInitWindowSize(640, 480);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
}
model::model(
  std::string filename,
  glm::vec3 scale,
  glm::vec3 position,
  glm::quat orientation,
  glm::vec3 velocity,
  glm::quat rotation)
  : filename(filename)
  , object(position, orientation, velocity, rotation)
  , scale(glm::scale(scale))
  , buffer_ids(std::vector<unsigned>())
  , id_counter(0) {
  glGenVertexArrays(1, &this->id);
}
model::model(std::string filename, glm::vec3 size, glm::vec3 position, glm::quat orientation, glm::vec3 velocity)
  : model(filename, size, position, orientation, velocity, glm::quat(0, 0, 0, 0)) {}
model::model(std::string filename, glm::vec3 size, glm::vec3 position, glm::quat orientation)
  : model(filename, size, position, orientation, glm::vec3(0, 0, 0)) {}
model::model(std::string filename, glm::vec3 size, glm::vec3 position)
  : model(filename, size, position, glm::quat(0, 0, 0, 0)) {}
model::model(std::string filename, glm::vec3 size)
  : model(filename, size, glm::vec3(0, 0, 0)) {}
model::model(std::string filename)
  : model(filename, glm::vec3(1, 1, 1)) {}
view::view(
  glm::vec3 position,
  glm::quat orientation,
  glm::vec3 velocity,
  glm::quat rotation)
  : object(position, orientation, velocity, rotation) {}
view::view(glm::vec3 position, glm::quat orientation, glm::vec3 velocity)
  : view(position, orientation, velocity, glm::quat(0, 0, 0, 0)) {}
view::view(glm::vec3 position, glm::quat orientation)
  : view(position, orientation, glm::vec3(0, 0, 0)) {}
view::view(glm::vec3 position)
  : view(position, glm::quat(0, 0, 0, 0)) {}
view::view()
  : view(glm::vec3(0, 0, 0)) {}
projection::projection(
  float fov,
  float aspect,
  float z_near,
  float z_far)
  : fov(fov)
  , aspect(aspect)
  , z_near(z_near)
  , z_far(z_far)
  , matrix(glm::perspective(fov , aspect, z_near, z_far)) {}
projection::projection(
  float left,
  float right,
  float bottom,
  float top,
  float z_near,
  float z_far)
  : projection(90.0f , (right - left)/(top - bottom), z_near, z_far) {}

window& window::operator+=(callback::close callback) {
  glutCloseFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::display callback) {
  glutDisplayFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::idle callback) {
  glutIdleFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::reshape callback) {
  glutReshapeFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::keyboard callback) {
  glutKeyboardFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::motion callback) {
  glutMotionFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::mouse callback) {
  glutMouseFunc(callback.handler);
  return (*this);
}
window& window::operator+=(callback::timer callback) {
  glutTimerFunc(
    callback.time,
    callback.handler,
    callback.value
  );
  return (*this);
}
window& window::operator+=(title title) {
  glutSetWindow(this->id);
  glutSetWindowTitle(title.name);
  return (*this);
}

template<mechanic m, behaviour b>
model& model::operator+=(movement<m, b> movement) {
  this->object += movement;
  this->matrix = glm::rotate(this->scale, glm::angle(this->object.orientation), glm::axis(this->object.orientation));
  this->matrix = glm::translate(this->matrix, this->object.position);
  return (*this);
}
template<mechanic m, behaviour b>
view& view::operator+=(movement<m, b> movement) {
  this->object += movement;
  this->matrix = glm::rotate(this->scale, glm::angle(this->object.orientation), glm::axis(this->object.orientation));
  this->matrix = glm::translate(this->matrix, this->object.position);
  return (*this);
}
projection& projection::operator+=(window window) {
  this->aspect = (float)window.width / (float)window.height;
  this->matrix = glm::perspective(this->fov, this->aspect, this->z_near, this->z_far);
  return (*this);
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

template<shader_type type>
program& program::operator+=(shader<type> shader) {
  std::ifstream shader_ifstream(shader.filename);
  std::string shader_string{
    std::istreambuf_iterator<char>{shader_ifstream},
    {}
  };
  const GLchar* shader_source = shader_string.c_str();

  auto shader_type = to_glenum(type);
  auto shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &shader_source, 0);
  glCompileShader(shader_id);
  glAttachShader(this->id, shader_id);

  this->shader_ids.push_back(shader_id);

  return (*this);
}
template program& program::operator+=(shader<compute>);
template program& program::operator+=(shader<vertex>);
template program& program::operator+=(shader<geometry>);
template program& program::operator+=(shader<tess_control>);
template program& program::operator+=(shader<tess_evaluation>);
template program& program::operator+=(shader<fragment>);
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

engine& engine::operator+=(window window) {
  if (this->window_ids[window.id]) {
    glutPostRedisplay();
  } else {
    glutMainLoop();
    this->window_ids.push_back(true);
  }
  return (*this);
}
engine& engine::operator+=(program program) {
  if (this->program_ids[program.id]) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program.id);
  } else {
    glLinkProgram(program.id);
    for (auto shader_id : program.shader_ids) {
      glDetachShader(program.id, shader_id);
    }
    this->program_ids.push_back(true);
  }
  return (*this);
}
engine& engine::operator+=(model model) {
  if (this->model_ids[model.id]) {
    glBindVertexArray(model.id);
    this->id_counter = model.id_counter;
  } else {
    Assimp::Importer importer;
    auto *scene = importer.ReadFile(model.filename, NULL);

    //for (auto i = 0u; i < scene->mNumMeshes; ++i) {
    auto *mesh = scene->mMeshes[0];

    glBindVertexArray(model.id);

    unsigned array_buffer_id;
    glGenBuffers(1, &array_buffer_id);
    model.buffer_ids.push_back(array_buffer_id);

    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 9 * sizeof(float), nullptr, GL_STATIC_DRAW);

    unsigned offset_counter;
    int size;
    unsigned buffer_size;

    // Insert vertex position data
    offset_counter = 0;
    size = 3;
    buffer_size = mesh->mNumVertices * size;
    auto *position_buffer = new float[buffer_size];
    for (auto i = 0u; i < mesh->mNumVertices; ++i) {
      position_buffer[i * size] = mesh->mVertices[i].x;
      position_buffer[i * size + 1] = mesh->mVertices[i].y;
      position_buffer[i * size + 2] = mesh->mVertices[i].z;
    }
    glBufferSubData(GL_ARRAY_BUFFER, offset_counter, buffer_size, position_buffer);
    glEnableVertexAttribArray(this->id_counter);
    glVertexAttribPointer(this->id_counter, size, GL_FLOAT, GL_FALSE, 0, (void*)offset_counter);
    delete[] position_buffer;
    this->id_counter += 1;

    // Insert vertex texture data
    //  offset_counter += buffer_size;
    //  size = 2;
    //  buffer_size = mesh->mNumVertices * size;
    //  auto *texture_buffer = new float[buffer_size];
    //  for (auto i = 0u; i < mesh->mNumVertices; ++i) {
    //    texture_buffer[i * size] = mesh->mTextureCoords[0][i].x;
    //    texture_buffer[i * size + 1] = mesh->mTextureCoords[0][i].y;
    //  }
    //  glBufferSubData(GL_ARRAY_BUFFER, offset_counter, buffer_size, texture_buffer);
    //  glEnableVertexAttribArray(this->id_counter);
    //  glVertexAttribPointer(this->id_counter, size, GL_FLOAT, GL_FALSE, 0, (void*)offset_counter);
    //  delete[] texture_buffer;
    //  this->id_counter += 1u;

    // Insert vertex normal data
    offset_counter += buffer_size;
    size = 3;
    buffer_size = mesh->mNumVertices * size;
    auto *normal_buffer = new float[buffer_size];
    for (auto i = 0u; i < mesh->mNumVertices; ++i) {
      normal_buffer[i * size] = mesh->mNormals[i].x;
      normal_buffer[i * size + 1] = mesh->mNormals[i].y;
      normal_buffer[i * size + 2] = mesh->mNormals[i].z;
    }
    glBufferSubData(GL_ARRAY_BUFFER, offset_counter, buffer_size, normal_buffer);
    glEnableVertexAttribArray(this->id_counter);
    glVertexAttribPointer(this->id_counter, size, GL_FLOAT, GL_FALSE, 0, (void*)offset_counter);
    delete[] normal_buffer;
    this->id_counter += 1;


    // Insert element buffer
    unsigned index_buffer_id;
    glGenBuffers(1, &index_buffer_id);
    model.buffer_ids.push_back(index_buffer_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    auto *element_buffer = new unsigned[mesh->mNumFaces * 3];
    for (auto i = 0u; i < mesh->mNumFaces; ++i) {
      element_buffer[i * 3] = mesh->mFaces[i].mIndices[0];
      element_buffer[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
      element_buffer[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(unsigned), element_buffer, GL_STATIC_DRAW);
    delete[] element_buffer;

    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Unbind buffers and arrays
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //}
  }
  return (*this);
}
engine& engine::operator+=(glm::mat4 buffer) {
  glUniformMatrix4fv(this->id_counter, 16, GL_FALSE, glm::value_ptr(buffer));
  return (*this);
}
engine& engine::operator-=(model model) {
  glBindVertexArray(0);
  return (*this);
}
engine& engine::operator-=(program program) {
  glutSwapBuffers();
  return (*this);
}
