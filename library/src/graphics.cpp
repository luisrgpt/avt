#include <gl/graphics.hpp>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace gl;

window::window(int *argc, char **argv, std::string title, unsigned width, unsigned height)
  : width(width)
  , height(height) {
  // Setup GLUT
  glutInit(argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  this->id = glutCreateWindow(title.c_str());

  // Setup GLEW
  // TODO: check why glew must initialize after creating a window
  glewExperimental = GL_TRUE;
  GLenum result = glewInit();

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
}
program::program()
  : id(glCreateProgram())
  , shader_ids(std::vector<unsigned>()) {}
engine::engine()
  : window_ids(boost::container::vector<bool>())
  , program_ids(boost::container::vector<bool>())
  , model_ids(boost::container::vector<bool>())
  , id_counter(0) {
  this->window_ids.push_back(false);
  this->program_ids.push_back(false);
  this->model_ids.push_back(false);
  this->model_sizes.push_back(0);
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
  , id_counter(0u)
  , matrix(glm::translate(position) * (glm::mat4_cast(orientation) * this->scale)) {
  glGenVertexArrays(1, &this->id);
}
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
  float z_far)
  : type(type)
  , left(left)
  , right(right)
  , bottom(bottom)
  , top(top)
  , z_near(z_near)
  , z_far(z_far) {
    if (type == perspective) {
      this->matrix = glm::frustum(left, right, bottom, top, z_near, z_far);
    }
    else {
      this->matrix = glm::ortho(left, right, bottom, top, z_near, z_far);
    }
  }

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
  return (*this);
}
template view& view::operator+=(movement<kinematic, linear>);
projection& projection::operator+=(window window) {
  glViewport(0, 0, window.width, window.height);

  float width = window.width / 2.0f;
  float height = window.height / 2.0f;
  if (type == perspective) {
    this->matrix = glm::frustum(
      -width, width,
      -height, height,
      10.0f, 1000.0f
    );
  }
  else {
    this->matrix = glm::ortho(
      -width, width,
      -height, height,
      10.0f, 1000.0f
    );
  }

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
  if (this->window_ids.size() > window.id && this->window_ids[window.id]) {
    glutPostRedisplay();
  } else {
    this->window_ids.insert(this->window_ids.begin() + window.id, true);
    glutMainLoop();
  }
  return (*this);
}
engine& engine::operator+=(program program) {
  if (this->program_ids.size() > program.id && this->program_ids[program.id]) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program.id);
  } else {
    glLinkProgram(program.id);
    glValidateProgram(program.id);
    for (auto shader_id : program.shader_ids) {
      glDetachShader(program.id, shader_id);
    }
    this->program_ids.insert(this->program_ids.begin() + program.id, true);
  }
  return (*this);
}
engine& engine::operator+=(model model) {
  if (this->model_ids.size() > model.id && this->model_ids[model.id]) {
    glBindVertexArray(model.id);
    this->id_counter = model.id;
  } else {
    Assimp::Importer importer;
    auto *scene = importer.ReadFile(model.filename, NULL);
    //for (auto i = 0u; i < scene->mNumMeshes; ++i) {
    auto *mesh = scene->mMeshes[0];

    // create array with faces
    // have to convert from Assimp format to array
    unsigned buffer;
    unsigned int *faceArray;
    faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
    unsigned int faceIndex = 0;

    for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
      const aiFace* face = &mesh->mFaces[t];

      memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
      faceIndex += 3;
    }

    // generate Vertex Array for mesh
    glBindVertexArray(model.id);

    // buffer for faces
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

    // buffer for vertex positions
    if (mesh->HasPositions()) {
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    }

    // buffer for vertex normals
    //if (mesh->HasNormals()) {
    //  glGenBuffers(1, &buffer);
    //  glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
    //  glEnableVertexAttribArray(1);
    //  glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
    //}

    // buffer for vertex texture coordinates
    //if (mesh->HasTextureCoords(0)) {
    //  float *texCoords = (float *)malloc(sizeof(float) * 2 * mesh->mNumVertices);
    //  for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

    //    texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
    //    texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

    //  }
    //  glGenBuffers(1, &buffer);
    //  glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
    //  glEnableVertexAttribArray(2);
    //  glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, 0);
    //}

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->model_ids.insert(this->model_ids.begin() + model.id, true);
    this->model_sizes.insert(this->model_sizes.begin() + model.id, mesh->mNumFaces * 3);
  }
  return (*this);
}
engine& engine::operator+=(glm::mat4 buffer) {
  glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(buffer));
  glDrawElements(GL_TRIANGLES, this->model_sizes[this->id_counter], GL_UNSIGNED_INT, NULL);

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
