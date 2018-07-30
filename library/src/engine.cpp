#include "engine.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
//#include <gli/gli.hpp>

using namespace gl;

engine::engine(
  int *argc,
  char **argv,
  std::string title,
  unsigned width,
  unsigned height)
  : window_ids(boost::container::vector<bool>())
  , programs(std::map<unsigned, program_info>())
  , mesh_ids(boost::container::vector<bool>())
  , vbo_ids(std::vector<std::vector<unsigned>>())
  , current_mesh_id(0)
  , width(width)
  , height(height) {
  this->window_ids.push_back(false);
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
  for (auto x : this->programs) {
    if (x.second.state != deleted) {
      x.second.state = deleted;
      glDeleteProgram(x.first);
      for (auto shader : x.second.shader_ids) {
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

bool engine::left_button_is_down(int button, int state) {
  return button == GLUT_LEFT_BUTTON && state == GLUT_DOWN;
}
bool engine::left_button_is_up(int button, int state) {
  return button == GLUT_LEFT_BUTTON && state == GLUT_UP;
}
bool engine::right_button_is_down(int button, int state) {
  return button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN;
}
bool engine::right_button_is_up(int button, int state) {
  return button == GLUT_RIGHT_BUTTON && state == GLUT_UP;
}

int gl::engine::get_width()
{
  return glutGet(GLUT_WINDOW_WIDTH);
}

int gl::engine::get_height()
{
  return glutGet(GLUT_WINDOW_HEIGHT);
}

void engine::reshape(float width, float height) {
  glViewport(0, 0, width, height);
}

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
void engine::set_keyboard_up_callback(void(*handler)(unsigned char, int, int)) {
  glutKeyboardUpFunc(handler);
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
void engine::end() {
  glutLeaveMainLoop();
}

program* engine::create_program() {
  unsigned id = glCreateProgram();
  assert(glGetError() == GL_NO_ERROR);

  this->programs.insert_or_assign(id, program_info());
  return new program{ id };
}
template<shader_type type>
void engine::load_shader(program program, std::string filename) {
  assert(this->programs[program.id].state == created);

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

  this->programs[program.id].shader_filenames.insert_or_assign(type, filename);
  this->programs[program.id].shader_ids.insert_or_assign(type, shader_id);
}
template void engine::load_shader<compute>(program program, std::string);
template void engine::load_shader<vertex>(program program, std::string);
template void engine::load_shader<geometry>(program program, std::string);
template void engine::load_shader<tess_control>(program program, std::string);
template void engine::load_shader<tess_evaluation>(program program, std::string);
template void engine::load_shader<fragment>(program program, std::string);
uniform* engine::get_uniform(program program, std::string name) {
  assert(this->programs[program.id].state == created);

  int id = glGetUniformLocation(program.id, name.c_str());
  assert(glGetError() == GL_NO_ERROR);

  return new uniform{ id };
}
uniform_block* engine::get_uniform_block(program program, std::string name) {
  assert(this->programs[program.id].state == created);

  unsigned id = glGetUniformBlockIndex(program.id, name.c_str());
  assert(glGetError() == GL_NO_ERROR);

  return new uniform_block{ id };
}
void engine::link(program program) {
  assert(this->programs[program.id].state == created);

  glLinkProgram(program.id);
  assert(glGetError() == GL_NO_ERROR);

  glValidateProgram(program.id);
  assert(glGetError() == GL_NO_ERROR);

  for (auto shader : programs[program.id].shader_ids) {
    glDetachShader(program.id, shader.second);
  }
  assert(glGetError() == GL_NO_ERROR);

  GLint test;
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &test);
  std::cout << "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT  " << test << std::endl;

  this->programs[program.id].state = linked;
}

template<class vertex, class fragment>
scene* engine::load_scene(program program, fs::obj obj, fs::mtl mtl) {
  assert(this->programs[program.id].state == linked);

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
      std::vector<GLfloat> v;
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
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), v.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
      assert(glGetError() == GL_NO_ERROR);
    }

    // buffer for vertex normals
    if (obj.f[n][0][2] > 0) {
      std::vector<GLfloat> vn;
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
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vn.size(), vn.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
      assert(glGetError() == GL_NO_ERROR);
    }

    // buffer for vertex texture coordinates
    if (obj.f[n][0][1] > 0) {
      std::vector<GLfloat> vt;
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
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vt.size(), vt.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, 0);
      assert(glGetError() == GL_NO_ERROR);
    }

    constexpr unsigned size =
    (
      sizeof(vertex) == 1
      ? 0
      : sizeof(vertex) % 16 != 0
      ? (sizeof(vertex) / 16 + 1) * 16
      : sizeof(vertex)
    ) + (
      sizeof(fragment) == 1
      ? 0
      : sizeof(fragment) % 16 != 0
      ? (sizeof(fragment) / 16 + 1) * 16
      : sizeof(fragment)
    );

    // Generate uniform buffer
    unsigned mvp_id;
    glGenBuffers(1, &mvp_id);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mvp_id);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, mvp_id);
    glBufferData(GL_UNIFORM_BUFFER, size, 0, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    assert(glGetError() == GL_NO_ERROR);

    constexpr vertex_type vertex_type
      = std::is_same<vertex, none>::value ? v_none
      : std::is_same<vertex, flat_vertex>::value ? v_flat
      : v_blinn_phong_vertex;

    constexpr fragment_type fragment_type
      = std::is_same<fragment, none>::value ? f_none
      : std::is_same<fragment, flat_fragment>::value ? f_flat
      : f_blinn_phong_fragment;

    this->mesh_ids.insert(this->mesh_ids.begin() + id, true);
    std::string usemtl = obj.usemtl[n];
    if (mtl.filename.find(obj.mtllib) != std::string::npos && usemtl.compare("None") != 0) {
      for (auto i = 0u; i < mtl.newmtl.size(); i++) {
        if (mtl.newmtl[i].compare(usemtl) == 0) {
          scene->push_back(
            mesh{
              regular,
              vertex_type,
              fragment_type,
              this->programs[program.id].shader_filenames[gl::vertex],
              this->programs[program.id].shader_filenames[gl::fragment],
              obj.filename,
              mtl.filename,

              id,
              obj.f[n].size(),
              program.id,
              0,
              0,
              mvp_id,
              mvp_id,

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
          plain,

          vertex_type,
          fragment_type,
          this->programs[program.id].shader_filenames[gl::vertex],
          this->programs[program.id].shader_filenames[gl::fragment],
          obj.filename,
          mtl.filename,

          id,
          obj.f[n].size(),
          program.id,
          0,
          0,
        }
      );
    }
  }
  return scene;
}
template scene* engine::load_scene<blinn_phong_vertex, blinn_phong_fragment>(program program, fs::obj obj, fs::mtl mtl);
template scene* engine::load_scene<particles_vertex, particles_fragment>(program program, fs::obj obj, fs::mtl mtl);
template scene* engine::load_scene<flat_vertex, flat_fragment>(program program, fs::obj obj, fs::mtl mtl);

//texture* engine::load_texture(program program, std::string filename) {
//  gli::texture Texture = gli::load(filename);
//  if (Texture.empty())
//    return 0;
//
//  gli::gl GL(gli::gl::PROFILE_GL33);
//  gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
//  GLenum Target = GL.translate(Texture.target());
//
//  GLuint TextureName = 0;
//  glGenTextures(1, &TextureName);
//  glBindTexture(Target, TextureName);
//  glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
//  glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
//  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
//  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
//  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
//  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);
//
//  glm::tvec3<GLsizei> const Extent(Texture.extent());
//  GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
//
//  switch (Texture.target())
//  {
//  case gli::TARGET_1D:
//    glTexStorage1D(
//      Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x);
//    break;
//  case gli::TARGET_1D_ARRAY:
//  case gli::TARGET_2D:
//  case gli::TARGET_CUBE:
//    glTexStorage2D(
//      Target, static_cast<GLint>(Texture.levels()), Format.Internal,
//      Extent.x, Texture.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
//    break;
//  case gli::TARGET_2D_ARRAY:
//  case gli::TARGET_3D:
//  case gli::TARGET_CUBE_ARRAY:
//    glTexStorage3D(
//      Target, static_cast<GLint>(Texture.levels()), Format.Internal,
//      Extent.x, Extent.y,
//      Texture.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
//    break;
//  default:
//    assert(0);
//    break;
//  }
//
//  for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
//    for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
//      for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
//      {
//        GLsizei const LayerGL = static_cast<GLsizei>(Layer);
//        glm::tvec3<GLsizei> Extent(Texture.extent(Level));
//        Target = gli::is_target_cube(Texture.target())
//          ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
//          : Target;
//
//        switch (Texture.target())
//        {
//        case gli::TARGET_1D:
//          if (gli::is_compressed(Texture.format()))
//            glCompressedTexSubImage1D(
//              Target, static_cast<GLint>(Level), 0, Extent.x,
//              Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
//              Texture.data(Layer, Face, Level));
//          else
//            glTexSubImage1D(
//              Target, static_cast<GLint>(Level), 0, Extent.x,
//              Format.External, Format.Type,
//              Texture.data(Layer, Face, Level));
//          break;
//        case gli::TARGET_1D_ARRAY:
//        case gli::TARGET_2D:
//        case gli::TARGET_CUBE:
//          if (gli::is_compressed(Texture.format()))
//            glCompressedTexSubImage2D(
//              Target, static_cast<GLint>(Level),
//              0, 0,
//              Extent.x,
//              Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
//              Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
//              Texture.data(Layer, Face, Level));
//          else
//            glTexSubImage2D(
//              Target, static_cast<GLint>(Level),
//              0, 0,
//              Extent.x,
//              Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
//              Format.External, Format.Type,
//              Texture.data(Layer, Face, Level));
//          break;
//        case gli::TARGET_2D_ARRAY:
//        case gli::TARGET_3D:
//        case gli::TARGET_CUBE_ARRAY:
//          if (gli::is_compressed(Texture.format()))
//            glCompressedTexSubImage3D(
//              Target, static_cast<GLint>(Level),
//              0, 0, 0,
//              Extent.x, Extent.y,
//              Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
//              Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
//              Texture.data(Layer, Face, Level));
//          else
//            glTexSubImage3D(
//              Target, static_cast<GLint>(Level),
//              0, 0, 0,
//              Extent.x, Extent.y,
//              Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
//              Format.External, Format.Type,
//              Texture.data(Layer, Face, Level));
//          break;
//        default: assert(0); break;
//        }
//      }
//  return new texture{ TextureName };
//}

void engine::use(program program) {
  assert(this->programs[program.id].state == linked);

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
  glUniformMatrix4fv(uniform.id, 1, GL_FALSE, buffer.values.data());
}

void engine::before_draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void engine::draw(mesh scene) {
  assert(this->current_mesh_id == scene.id);

  glDrawElements(GL_TRIANGLES, scene.n_faces, GL_UNSIGNED_INT, NULL);
}
void engine::draw(
  graph<model> scene_graph,
  camera camera,
  directional_light d_light
//) {
//  this->draw(
//    std::cref(scene_graph),
//    std::cref(camera.projection.matrix),
//    std::cref(camera.view.matrix)
//  );
//}
//void engine::draw(
//  graph<model> scene_graph,
//  free_camera camera
//) {
//  this->draw(
//    std::cref(scene_graph),
//    std::cref(camera.projection.matrix),
//    std::cref(camera.view.matrix)
//  );
//}
//void engine::draw(
//  graph<model> scene_graph,
//  math::matrix_4d projection,
//  math::matrix_4d view
) {
  illumination lights;
  std::vector<math::vector_4d> lights_position;
  std::vector<math::matrix_4d> translations = { math::matrix_4d::kIdentity };
  std::vector<math::matrix_4d> rotations = { math::matrix_4d::kIdentity };
  std::vector<math::matrix_4d> scalations = { math::matrix_4d::kIdentity };

  // Calculate matrices
  for (auto i = 1u; i < scene_graph.nodes.size(); i++) {
    auto node = scene_graph.nodes[i].value();
    auto parent_id = scene_graph.edges[i].id;

    switch (node.scene_info[0].type) {
      case invisible:
        translations.push_back(translations[parent_id]);
        rotations.push_back(rotations[parent_id]);
        scalations.push_back(scalations[parent_id]);
        break;
      case billboard:
        translations.push_back(math::matrix_3d::Translation(node.object.position.x, node.object.position.y, node.object.position.z) * translations[parent_id]);
        rotations.push_back(math::matrix_4d(camera.view.object.orientation));
        scalations.push_back((camera.view.object.position - node.object.position).Norm() * node.scale/* * scalations[parent_id]*/);
        break;
      default:
        translations.push_back(math::matrix_3d::Translation(node.object.position.x, node.object.position.y, node.object.position.z) * translations[parent_id]);
        rotations.push_back(math::matrix_4d(node.object.orientation) * rotations[parent_id]);
        scalations.push_back(node.scale/* * scalations[parent_id]*/);
        break;
    }

    if (node.scene_info[0].type == light) {
      math::vector_4d vector4(translations.back()[3][0], translations.back()[3][1], translations.back()[3][2], 1.0f);

      lights.push_back(node);
      lights_position.push_back(vector4);
    }
  }

  // Draw meshes
  for (auto i = 1u; i < scene_graph.nodes.size(); i++) {
    for (auto mesh : scene_graph.nodes[i].value().scene_info) {
      if (mesh.type == light || mesh.type == invisible) {
        continue;
      }

      if (this->current_program_id != mesh.program_id) {
        assert(this->programs[mesh.program_id].state == linked);

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

      switch (mesh.vertex_type) {
      case v_none:
        
        break;
      case v_flat:
        this->set_flat(
          mesh,
          camera.projection.matrix,
          camera.view.matrix,
          translations[i],
          rotations[i],
          scalations[i]
        );
        break;
      case v_particles:
        this->set_particles(
          current_program_id,
          mesh, lights,
          camera.projection.matrix,
          camera.view.matrix,
          translations[i],
          rotations[i],
          scalations[i]
        );
        break;
      case v_blinn_phong_vertex:
        this->set_blinn_phong(
          current_program_id,
          mesh, lights,
          d_light,
          lights_position,
          camera.projection.matrix,
          camera.view.matrix,
          translations[i],
          rotations[i],
          scalations[i]
        );
        break;
      }

      switch (mesh.type) {
      case particles:
        glDrawArrays(GL_POINTS, 0, (GLsizei)mesh.n_faces * 3);
        break;

      default:
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.n_faces * 3);
        break;
      }
      assert(glGetError() == GL_NO_ERROR);
    }
  }
}
void engine::after_draw() {
  glutSwapBuffers();
}

void engine::set_flat(
  mesh mesh,
  math::matrix_4d projection,
  math::matrix_4d view,
  math::matrix_4d translation,
  math::matrix_4d rotation,
  math::matrix_4d scalation
) {
  math::matrix_4d model = translation * rotation * scalation;

  glBindBufferRange(GL_UNIFORM_BUFFER, 0, mesh.vertex, 0, sizeof(GLfloat) * 60);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat) * 16, model.values.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16, sizeof(GLfloat) * 16, view.values.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 32, sizeof(GLfloat) * 16, projection.values.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 48, sizeof(GLfloat) * 9, (view * model).Normal().values.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 57, sizeof(GLfloat) * 3, std::vector<GLfloat>{0.0f,0.0f,0.0f}.data());
  glBindBufferRange(GL_UNIFORM_BUFFER, 1, mesh.fragment, sizeof(GLfloat) * 60, sizeof(GLfloat) * 13 + sizeof(GLint));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 60, sizeof(GLfloat) * 3, mesh.ambient.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 63, sizeof(GLfloat) * 3, mesh.diffuse.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 66, sizeof(GLfloat) * 3, mesh.specular.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 69, sizeof(GLfloat) * 3, mesh.emissive.data());
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 72, sizeof(GLfloat), &mesh.shininess);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 73, sizeof(GLint), &mesh.n_textures);
  glBindBuffer(GL_UNIFORM_BUFFER, GL_ZERO);
  assert(glGetError() == GL_NO_ERROR);
}


void engine::set_particles(
  unsigned id,
  mesh mesh,
  illumination lights,
  math::matrix_4d projection,
  math::matrix_4d view,
  math::matrix_4d translation,
  math::matrix_4d rotation,
  math::matrix_4d scalation
) {
  math::matrix_4d model = translation * rotation * scalation;

  auto loc = glGetUniformLocation(id, "model"); glUniformMatrix4fv(loc, 1, GL_FALSE, model.values.data());
  loc = glGetUniformLocation(id, "view"); glUniformMatrix4fv(loc, 1, GL_FALSE, view.values.data());
  loc = glGetUniformLocation(id, "projection"); glUniformMatrix4fv(loc, 1, GL_FALSE, projection.values.data());
  loc = glGetUniformLocation(id, "dispersion0"); glUniform3fv(loc, 1, std::array<float, 3>{(rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10}.data());
  loc = glGetUniformLocation(id, "dispersion1"); glUniform3fv(loc, 1, std::array<float, 3>{(rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10}.data());
  loc = glGetUniformLocation(id, "dispersion2"); glUniform3fv(loc, 1, std::array<float, 3>{(rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10}.data());
  loc = glGetUniformLocation(id, "dispersion3"); glUniform3fv(loc, 1, std::array<float, 3>{(rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10, (rand() % 2 - 1.0f) / 10}.data());

}

void engine::set_blinn_phong(
  unsigned id,
  mesh mesh,
  illumination lights,
  directional_light d_light,
  std::vector<math::vector_4d> lights_position,
  math::matrix_4d projection,
  math::matrix_4d view,
  math::matrix_4d translation,
  math::matrix_4d rotation,
  math::matrix_4d scalation
) {
  math::matrix_4d model = translation * rotation * scalation;

  constexpr int t = sizeof(GLfloat) * 67 + sizeof(GLint);

  auto mode = (GLint)(mesh.type == textured);

  //std::vector<GLfloat>{lights[0].x, lights[0].y, lights[0].z}.data();

  
  //auto carro = aux3 + lights[0].object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f);

  #define DEGREES_TO_RADIANS 0.01745329251994329547

  auto aux = lights_position[0]; std::array<GLfloat, 3> light_position0 = { aux.x, aux.y, aux.z };
  aux = lights_position[1]; std::array<GLfloat, 3> light_position1 = { aux.x, aux.y, aux.z };
  aux = lights_position[2]; std::array<GLfloat, 3> light_position2 = { aux.x, aux.y, aux.z };
  aux = lights_position[3]; std::array<GLfloat, 3> light_position3 = { aux.x, aux.y, aux.z };
  aux = lights_position[4]; std::array<GLfloat, 3> light_position4 = { aux.x, aux.y, aux.z };
  //std::array<GLfloat, 3> light_direction = { carro.x, carro.y, carro.z };
  std::array<GLfloat, 3> light_direction = { 0,0,0 };
  std::array<GLfloat, 3> light_color = {1, 1, 1};
  GLfloat light_Ka = 0.5;
  GLfloat light_angle = 20 * DEGREES_TO_RADIANS;
  GLfloat light_attenuation = 0.1;
  //constexpr int aaaa = sizeof(GLfloat) * 73 + sizeof(GLint) + 8;
  //constexpr int aaaa = sizeof(blinn_phong_vertex);;
  constexpr int aaaa = sizeof(blinn_phong_fragment);

  //std::cout << light_direction[0] << " " << light_direction[1] << " " << light_direction[2] << std::endl;


  constexpr unsigned size = sizeof(GLfloat) * 16 + sizeof(GLint) * 2 + 6;

  int on_or_off0 = lights[0].object.velocity.x;
  int on_or_off1 = lights[1].object.velocity.x;
  int on_or_off2 = lights[2].object.velocity.x;
  int on_or_off3 = lights[3].object.velocity.x;
  int on_or_off4 = lights[4].object.velocity.x;

  int is_main0 = lights[0].object.velocity.y;
  int is_main1 = lights[1].object.velocity.y;
  int is_main2 = lights[2].object.velocity.y;
  int is_main3 = lights[3].object.velocity.y;
  int is_main4 = lights[4].object.velocity.y;

  auto loc = glGetUniformLocation(id, "model"); glUniformMatrix4fv(loc, 1, GL_FALSE, model.values.data());
  loc = glGetUniformLocation(id, "view"); glUniformMatrix4fv(loc, 1, GL_FALSE, view.values.data());
  loc = glGetUniformLocation(id, "projection"); glUniformMatrix4fv(loc, 1, GL_FALSE, projection.values.data());
  loc = glGetUniformLocation(id, "m_normal"); glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::make_mat4((view * model).values.data())))));
  loc = glGetUniformLocation(id, "light_position0"); glUniform3fv(loc, 1, light_position0.data());
  loc = glGetUniformLocation(id, "light_position1"); glUniform3fv(loc, 1, light_position1.data());
  loc = glGetUniformLocation(id, "light_position2"); glUniform3fv(loc, 1, light_position2.data());
  loc = glGetUniformLocation(id, "light_position3"); glUniform3fv(loc, 1, light_position3.data());
  loc = glGetUniformLocation(id, "light_position4"); glUniform3fv(loc, 1, light_position4.data());
  loc = glGetUniformLocation(id, "light_Ka"); glUniform1fv(loc, 1, &light_Ka);
  loc = glGetUniformLocation(id, "v_mode"); glUniform1iv(loc, 1, &mode);

  loc = glGetUniformLocation(id, "Ka"); glUniform3fv(loc, 1, mesh.ambient.data());
  loc = glGetUniformLocation(id, "Kd"); glUniform3fv(loc, 1, mesh.diffuse.data());
  loc = glGetUniformLocation(id, "Ks"); glUniform3fv(loc, 1, mesh.specular.data());
  loc = glGetUniformLocation(id, "Ke"); glUniform3fv(loc, 1, mesh.emissive.data());
  loc = glGetUniformLocation(id, "shininess"); glUniform1fv(loc, 1, &mesh.shininess);
  loc = glGetUniformLocation(id, "text_count"); glUniform1iv(loc, 1, &mesh.n_textures);
  loc = glGetUniformLocation(id, "light_color"); glUniform3fv(loc, 1, light_color.data());
  loc = glGetUniformLocation(id, "f_mode"); glUniform1iv(loc, 1, &mode);
  loc = glGetUniformLocation(id, "light_attenuation"); glUniform1fv(loc, 1, &light_attenuation);
  loc = glGetUniformLocation(id, "light_angle"); glUniform1fv(loc, 1, &light_angle);
  loc = glGetUniformLocation(id, "on_or_off0"); glUniform1iv(loc, 1, &on_or_off0);
  loc = glGetUniformLocation(id, "on_or_off1"); glUniform1iv(loc, 1, &on_or_off1);
  loc = glGetUniformLocation(id, "on_or_off2"); glUniform1iv(loc, 1, &on_or_off2);
  loc = glGetUniformLocation(id, "on_or_off3"); glUniform1iv(loc, 1, &on_or_off3);
  loc = glGetUniformLocation(id, "on_or_off4"); glUniform1iv(loc, 1, &on_or_off4);
  loc = glGetUniformLocation(id, "is_main0"); glUniform1iv(loc, 1, &is_main0);
  loc = glGetUniformLocation(id, "is_main1"); glUniform1iv(loc, 1, &is_main1);
  loc = glGetUniformLocation(id, "is_main2"); glUniform1iv(loc, 1, &is_main2);
  loc = glGetUniformLocation(id, "is_main3"); glUniform1iv(loc, 1, &is_main3);
  loc = glGetUniformLocation(id, "is_main4"); glUniform1iv(loc, 1, &is_main4);
  loc = glGetUniformLocation(id, "directional_light"); glUniform3fv(loc, 1, std::array<float, 3>{ d_light.position.x, d_light.position.y, d_light.position.z }.data());
  loc = glGetUniformLocation(id, "directional_light_state"); glUniform1iv(loc, 1, &d_light.state);

  //glBindBufferRange(GL_UNIFORM_BUFFER, 1, mesh.fragment, sizeof(GLfloat) * 73 + sizeof(GLint) + 8, sizeof(GLfloat) * 16 + sizeof(GLint) * 2 + 8);
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 73 + sizeof(GLint) + 8, sizeof(GLfloat) * 3, mesh.ambient.data());
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 76 + sizeof(GLint) + 8, sizeof(GLfloat) * 3, mesh.diffuse.data());
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 79 + sizeof(GLint) + 8, sizeof(GLfloat) * 3, mesh.specular.data());
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 82 + sizeof(GLint) + 8, sizeof(GLfloat) * 3, mesh.emissive.data());
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 85 + sizeof(GLint) + 8, sizeof(GLfloat), &mesh.shininess);
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 86 + sizeof(GLint) + 8, sizeof(GLint), &mesh.n_textures);
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 86 + sizeof(GLint) * 2 + 8, sizeof(GLfloat) * 3, light_color.data());
  //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 89 + sizeof(GLint) * 2 + 8, sizeof(GLint), &mode);
  assert(glGetError() == GL_NO_ERROR);
}


void engine::unbind() {
  glBindVertexArray(0);

  this->current_mesh_id = 0;
}
void engine::unuse() {
  glutSwapBuffers();

  this->current_program_id = 0;
}
