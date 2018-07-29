#include <iostream>
#include <functional>
#include <cmath>

#include <glm/gtc/type_ptr.hpp>

#include "math/quaternion.hpp"
#include "math/vector_3d.hpp"
#include "engine.hpp"
#include "camera.hpp"
#include "graph_scene.hpp"
#include "mouse_picking.hpp"




int score = 0;
int lifes = 3;
int angle = 160;
float ball_speed = 0.3f;
float ship_speed = 0.8f;
float ship_size = 2.4f;





constexpr unsigned
fps = 60,
mhz = 1000 / fps,
width = 1000,
height = 800;

enum ship_state {
  none,
  left,
  right
};

enum object_property {
  fragile,
  corrosive,
  durable
};

gl::engine *engine;
gl::program *flat;
gl::program *blinn_phong;
gl::graph<gl::model> *scene_graph;
std::array<gl::camera, 3> *cameras;
gl::graph<gl::model> *hud;
gl::camera *hud_camera;
fs::mtl *random_materials;
gl::node *node_field;
gl::node *node_light;
gl::node *node_ship;
gl::node *node_ball;
std::vector<std::tuple<gl::node, object_property>> ball_obstacles;
gl::node *left_ship_obstacle;
gl::node *right_ship_obstacle;
unsigned current_camera;
ship_state state = none;
bool ball_is_attached = true;

gl::scene* number;
gl::scene *life;
gl::scene *scene_0;
gl::scene *scene_1;
gl::scene *scene_2;
gl::scene *scene_3;
gl::scene *scene_4;
gl::scene *scene_5;
gl::scene *scene_6;
gl::scene *scene_7;
gl::scene *scene_8;
gl::scene *scene_9;
gl::scene *scene_game_over;
gl::scene *scene_retry;

void update_hud() {
  delete hud;

  hud = new gl::graph<gl::model>();
  for (auto i = 0; i < lifes - 1; i++) {
    hud->set_root(
      gl::model(
        *life,
        math::vector_3d(0.072f, 0.03f, 0.03f),
        math::vector_3d(-0.85f + 0.12f * i, -0.9f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      )
    );
  }

  
  for (auto i = 0; i < (score > 0 ? (int)log10(score) + 1 : 1); i++) {
    switch (score / (int)std::powf(10, i) % 10) {
    case 0:
      number = scene_0;
      break;
    case 1:
      number = scene_1;
      break;
    case 2:
      number = scene_2;
      break;
    case 3:
      number = scene_3;
      break;
    case 4:
      number = scene_4;
      break;
    case 5:
      number = scene_5;
      break;
    case 6:
      number = scene_6;
      break;
    case 7:
      number = scene_7;
      break;
    case 8:
      number = scene_8;
      break;
    default:
      number = scene_9;
      break;
    }

    hud->set_root(
      gl::model(
        *number,
        math::vector_3d(0.1f, 0.1f, 0.1f),
        math::vector_3d(0.91 - 0.05f * i, 0.85f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      )
    );
  }

  if (lifes == 0) {
    hud->set_root(
      gl::model(
        *scene_game_over,
        math::vector_3d(0.3f, 0.3f, 0.3f),
        math::vector_3d(0.0f, 0.0f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      )
    );

    hud->set_root(
      gl::model(
        *scene_retry,
        math::vector_3d(0.2f, 0.2f, 0.2f),
        math::vector_3d(0.0f, -0.2f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      )
    );
  }
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void close() {
  delete engine;
}
void idle() {}
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case ' ':
    if (ball_is_attached) {
      scene_graph->edges[node_ball->id] = *node_field;
      scene_graph->nodes[node_ball->id].value().object.position.x += scene_graph->nodes[node_ship->id].value().object.position.x;
      scene_graph->nodes[node_ball->id].value().object.position.y -= 12.0f;
      scene_graph->nodes[node_ball->id].value().object.position.z += 1.0f;
      scene_graph->nodes[node_ball->id].value().execute(gl::rotation(math::quaternion((rand() % angle) - angle/2, scene_graph->nodes[node_ball->id].value().object.orientation * math::vector_3d(0.0f, 0.0f, 1.0f)), 1));
      ball_is_attached = false;
    }
    break;

  case '1':
    current_camera = 0;
    break;
  case '2':
    current_camera = 1;
    break;
  case '3':
    current_camera = 2;
    break;

  case 'a':
    switch (state) {
      case none:
        state = left;
        break;
      case right:
        state = none;
        break;
    }
    break;
  case 'd':
    switch (state) {
    case none:
      state = right;
      break;
    case left:
      state = none;
      break;
    }
    break;

  case 'j':
    scene_graph->nodes[node_field->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, 1.0f, 0.0f)), 1));
    break;
  case 'l':
    scene_graph->nodes[node_field->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, -1.0f, 0.0f)), 1));
    break;
  case 'k':
    scene_graph->nodes[node_field->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(-1.0f, 0.0f, 0.0f)), 1));
    break;
  case 'i':
    scene_graph->nodes[node_field->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(1.0f, 0.0f, 0.0f)), 1));
    break;
  case 'u':
    scene_graph->nodes[node_field->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, 0.0f, 1.0f)), 1));
    break;
  case 'o':
    scene_graph->nodes[node_field->id].value().object.execute(gl::rotation(math::quaternion(1, math::vector_3d(0.0f, 0.0f, -1.0f)), 1));
    break;
  }
}

void keyboard_up(unsigned char key, int x, int y) {
  switch (key) {
  case 'y':
    if (lifes == 0) {
      scene_graph = gl::bltz::load("start.bltz");
      score = 0;
      lifes = 3;
      ball_is_attached = true;
      update_hud();
    }
    break;
  case 'n':
    if (lifes == 0) {
      engine->end();
    }
    break;

  case 'a':
    switch (state) {
    case none:
      state = right;
      break;
    case left:
      state = none;
      break;
    }
    break;
  case 'd':
    switch (state) {
    case none:
      state = left;
      break;
    case right:
      state = none;
      break;
    }
  }
}

void mouse(int button, int state, int x, int y) {
  bool left_is_down = engine->left_button_is_down(button, state);

  if (left_is_down) {
  }
  else if (engine->left_button_is_up(button, state)) {
  }
}
void display() {
  auto ship = scene_graph->nodes[node_ship->id].value();
  auto left = scene_graph->nodes[left_ship_obstacle->id].value();
  auto right = scene_graph->nodes[right_ship_obstacle->id].value();
  auto left_side = left.object.position.x + left.scale[0][0] / 2;
  auto right_side = right.object.position.x - right.scale[0][0] / 2;
  switch (state) {
  case ship_state::left:
    if (left_side < ship.object.position.x - ship.scale[0][0] / 2) {
      scene_graph->nodes[node_ship->id].value().execute(gl::linear_movement(math::vector_3d(-ship_speed, 0.0f, 0.0f), 1.0));
      cameras->at(1).view.execute(gl::linear_movement(math::vector_3d(-ship_speed, 0.0f, 0.0f), 1));
      cameras->at(2).view.execute(gl::linear_movement(math::vector_3d(-ship_speed, 0.0f, 0.0f), 1));
    }
    break;
  case ship_state::right:
    if (ship.object.position.x + ship.scale[0][0] / 2 < right_side) {
      scene_graph->nodes[node_ship->id].value().execute(gl::linear_movement(math::vector_3d(ship_speed, 0.0f, 0.0f), 1.0));
      cameras->at(1).view.execute(gl::linear_movement(math::vector_3d(ship_speed, 0.0f, 0.0f), 1));
      cameras->at(2).view.execute(gl::linear_movement(math::vector_3d(ship_speed, 0.0f, 0.0f), 1));
    }
    break;
  }

  if (!ball_is_attached) {
    if (lifes != 0) {
      auto ball = scene_graph->nodes[node_ball->id].value();
      auto ball_center = ball.object.position;
      auto speed = scene_graph->nodes[node_ball->id].value().object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f);
      auto ball_left_side = ball_center.x - ball.scale[0][0] / 2;
      auto ball_right_side = ball_center.x + ball.scale[0][0] / 2;
      auto ball_bottom_side = ball_center.y - ball.scale[1][1] / 2;
      auto ball_top_side = ball_center.y + ball.scale[1][1] / 2;

      auto i = 0;
      for (auto obstacle : ball_obstacles) {
        auto object = scene_graph->nodes[std::get<0>(obstacle).id].value();
        auto object_left_side = object.object.position.x - object.scale[0][0] / 2;
        auto object_right_side = object.object.position.x + object.scale[0][0] / 2;
        auto object_bottom_side = object.object.position.y - object.scale[1][1] / 2;
        auto object_top_side = object.object.position.y + object.scale[1][1] / 2;

        auto x_collide = (object_bottom_side <= ball_center.y && ball_center.y <= object_top_side) &&
          ((speed.x < 0 && object_left_side <= ball_left_side && ball_left_side <= object_right_side) ||
          (speed.x > 0 && object_left_side <= ball_right_side && ball_right_side <= object_right_side));
        auto y_collide = (object_left_side <= ball_center.x && ball_center.x <= object_right_side) &&
          ((speed.y < 0 && object_bottom_side <= ball_bottom_side && ball_bottom_side <= object_top_side) ||
          (speed.y > 0 && object_bottom_side <= ball_top_side && ball_top_side <= object_top_side));

        if (x_collide) {
          scene_graph->nodes[node_ball->id].value().object.orientation.z = -ball.object.orientation.z;
          switch (std::get<1>(obstacle)) {
          case fragile:
            for (auto i = 0; i < object.scene_info.size(); i++) {
              scene_graph->nodes[std::get<0>(obstacle).id].value().scene_info[i].type = gl::invisible;
            }
            ball_obstacles.erase(ball_obstacles.begin() + i);
            score += 100;
            update_hud();
            break;
          case corrosive:
            lifes -= 1;
            update_hud();
            break;
          }
          break;
        }
        if (y_collide) {
          scene_graph->nodes[node_ball->id].value().object.orientation.z = -ball.object.orientation.z;
          scene_graph->nodes[node_ball->id].value().object.orientation *= math::quaternion(180, math::vector_3d(0.0f, 0.0f, 1.0f));
          switch (std::get<1>(obstacle)) {
          case fragile:
            for (auto i = 0; i < object.scene_info.size(); i++) {
              scene_graph->nodes[std::get<0>(obstacle).id].value().scene_info[i].type = gl::invisible;
            }
            ball_obstacles.erase(ball_obstacles.begin() + i);
            score += 100;
            update_hud();
            gl::bltz::save(*scene_graph, "save.bltz");
            break;
          case corrosive:
            lifes -= 1;
            update_hud();
            scene_graph = gl::bltz::load("save.bltz");
            ball_is_attached = true;
            scene_graph->edges[node_ball->id] = *node_ship;
            scene_graph->nodes[node_ball->id].value().object.position = math::vector_3d(0.0f, 0.8f * 0.6f, 0.0f);
            scene_graph->nodes[node_ball->id].value().object.orientation = math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f));
            cameras->at(1).view.object.position = scene_graph->nodes[node_ship->id].value().object.position + math::vector_3d(-23.5f, 14.5f, 0.0f) + math::vector_3d(0.0f, -4.5f, 3.0f);
            cameras->at(2).view.object.position = scene_graph->nodes[node_ship->id].value().object.position + math::vector_3d(-23.5f, 14.5f, 0.0f);
            break;
          }
          break;
        }
        i += 1;
      }

      scene_graph->nodes[node_ball->id].value().execute(gl::linear_movement(scene_graph->nodes[node_ball->id].value().object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f), ball_speed));
    }
  }

  engine->before_draw();
  if (lifes != 0) {
    engine->draw(
      *scene_graph,
      cameras->at(current_camera)
    );
  }
  engine->draw(
    *hud,
    *hud_camera
  );
  engine->after_draw();
}
int cnt = 0;
void timer(int value) {
  engine->set_timer_callback(mhz, timer, 0);
  engine->display();
}
void reshape(int width, int height) {
  engine->reshape(width, height);
  cameras->at(current_camera).projection.reshape(width, height);
}

int main(int argc, char **argv) {
  engine = new gl::engine(&argc, argv, "Arkanoid Level Editor", width, height);
  engine->set_close_callback(close);
  engine->set_display_callback(display);
  engine->set_idle_callback(idle);
  engine->set_reshape_callback(reshape);
  engine->set_keyboard_callback(keyboard);
  engine->set_keyboard_up_callback(keyboard_up);
  engine->set_mouse_callback(mouse);
  engine->set_timer_callback(0, timer, 0);

  blinn_phong = engine->create_program();
  engine->load_shader<gl::vertex>(*blinn_phong, "share/blinn_phong.vert");
  engine->load_shader<gl::fragment>(*blinn_phong, "share/blinn_phong.frag");
  engine->link(*blinn_phong);

  flat = engine->create_program();
  engine->load_shader<gl::vertex>(*flat, "share/flat.vert");
  engine->load_shader<gl::fragment>(*flat, "share/flat.frag");
  engine->link(*flat);

  random_materials = new fs::mtl("share/random_materials.mtl");

  fs::mtl materials("share/materials.mtl");
  fs::obj obj_field("share/field.obj");
  fs::obj obj_ship("share/ship.obj");
  fs::obj obj_ball("share/ball.obj");
  fs::obj obj_box("share/box.obj");
  fs::obj obj_pipe("share/big_pipe.obj");
  fs::obj obj_border("share/border.obj");

  fs::obj obj_0("share/0.obj");
  fs::obj obj_1("share/1.obj");
  fs::obj obj_2("share/2.obj");
  fs::obj obj_3("share/3.obj");
  fs::obj obj_4("share/4.obj");
  fs::obj obj_5("share/5.obj");
  fs::obj obj_6("share/6.obj");
  fs::obj obj_7("share/7.obj");
  fs::obj obj_8("share/8.obj");
  fs::obj obj_9("share/9.obj");
  fs::obj obj_retry("share/retry.obj");
  fs::obj obj_game_over("share/game_over.obj");

  gl::scene *field = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_field, materials);
  gl::scene *ship = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_ship, materials);
  gl::scene *ball = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_ball, materials);
  gl::scene *box = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_box, materials);
  gl::scene *big_pipe = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_pipe, materials);
  gl::scene *border = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_border, materials);
  gl::scene *wall = engine->load_scene<gl::blinn_phong_vertex, gl::blinn_phong_fragment>(*blinn_phong, obj_box, materials);

  life = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_ship, materials);
  scene_0 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_0, materials);
  scene_1 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_1, materials);
  scene_2 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_2, materials);
  scene_3 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_3, materials);
  scene_4 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_4, materials);
  scene_5 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_5, materials);
  scene_6 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_6, materials);
  scene_7 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_7, materials);
  scene_8 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_8, materials);
  scene_9 = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_9, materials);
  scene_retry = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_retry, materials);
  scene_game_over = engine->load_scene<gl::flat_vertex, gl::flat_fragment>(*flat, obj_game_over, materials);

  scene_graph = new gl::graph<gl::model>();
  node_field = scene_graph->set_root(
    gl::model(
      *field,
      math::vector_3d(1, 1, 1),
      math::vector_3d(-23.5f, 14.5f, 0.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  node_light = scene_graph->set_child(
    *node_field,
    gl::model(
      {gl::mesh{ gl::light }},
      math::vector_3d(0, 0, 0),
      math::vector_3d(0.0f, 0.0f, 0.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  ball_obstacles = std::vector<std::tuple<gl::node, object_property>>();
  node_ship = scene_graph->set_child(
    *node_field,
    gl::model(
      *ship,
      math::vector_3d(ship_size, 1.0f, 1.0f),
      math::vector_3d(0.0f, -12.0f, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  ball_obstacles.push_back(std::tuple(*node_ship, durable));
  node_ball = scene_graph->set_child(
    *node_ship,
    gl::model(
      *ball,
      math::vector_3d(0.6f, 0.6f, 0.6f),
      math::vector_3d(0.0f, 0.8f, 0.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );

  left_ship_obstacle = scene_graph->set_child(
    *node_field,
    gl::model(
      *wall,
      math::vector_3d(0.8f, 29.0f, 1.25f),
      math::vector_3d(-13.0f, 0.0f, 1.25f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  ball_obstacles.push_back(std::tuple(*left_ship_obstacle, durable));
  right_ship_obstacle = scene_graph->set_child(
    *node_field,
    gl::model(
      *wall,
      math::vector_3d(0.8f, 29.0f, 1.25f),
      math::vector_3d(13.0f, 0.0f, 1.25f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  ball_obstacles.push_back(std::tuple(*right_ship_obstacle, durable));
  auto up_obstacle = scene_graph->set_child(
    *node_field,
    gl::model(
      *wall,
      math::vector_3d(26.0f, 0.8f, 1.25f),
      math::vector_3d(0.0f, 14.5f, 1.25f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  ball_obstacles.push_back(std::tuple(*up_obstacle, durable));
  auto down_obstacle = scene_graph->set_child(
    *node_field,
    gl::model(
      *wall,
      math::vector_3d(26.0f, 1.0f, 1.25f),
      math::vector_3d(0.0f, -15.0f, 1.25f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );
  scene_graph->nodes[down_obstacle->id].value().scene_info[0].type = gl::invisible;
  ball_obstacles.push_back(std::tuple(*down_obstacle, corrosive));
  scene_graph->set_child(
    *node_field,
    gl::model(
      *border,
      math::vector_3d(1, 1, 1),
      math::vector_3d(0.0f, -0.2f, 2.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    )
  );

  for (auto i = 0u; i < 6u; i++) {
    for (auto j = 0u; j < 13u; j++) {
      auto box_obstacle = scene_graph->set_child(
        *node_field,
        gl::model(
          *box,
          math::vector_3d(1.8f, 0.8f, 0.8f),
          math::vector_3d(-11.5f + 1.9f * j, 3.95f + 0.85f * i, 1.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        )
      );
      ball_obstacles.push_back(std::tuple(*box_obstacle, fragile));
    }
  }

  for (auto i = 0u; i < 6u; i++) {
    for (auto j = 0u; j < 2u; j++) {
      scene_graph->set_child(
        *node_field,
        gl::model(
          *big_pipe,
          math::vector_3d(1, 1, 1),
          math::vector_3d(-13.0f + 26.0f * j, -12.625f + 5.0f * i, 2.0f), math::quaternion(0, math::vector_3d(0.0f, 1.0f, 0.0f)),
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
        math::vector_3d(-6.75f + 13.5f * j, 14.5f, 2.0f), math::quaternion(90, math::vector_3d(0.0f, 0.0f, 1.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      )
    );
  }

  cameras = new std::array<gl::camera, 3>{
    gl::camera{
      gl::view(
        math::vector_3d(-23.5f, 15.0f, 30.0f), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
        math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
      ),
      gl::orthogonal_projection(
        width, height,
        -13.5f, 13.5f,
        -15.0f, 15.0f,
        1.0f, 100.0f
      )
    },
      gl::camera{
        gl::view(
          math::vector_3d(-23.5f, -2.0f, 4.0f), math::quaternion(90, math::vector_3d(1.0f, 0.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        ),
        gl::perspective_projection(
          width, height,
          90,
          0.1f, 50.0f
        )
    },
      gl::camera{
        gl::view(
          math::vector_3d(-23.5f, 2.5f, 1.0f), math::quaternion(90, math::vector_3d(1.0f, 0.0f, 0.0f)),
          math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
        ),
        gl::perspective_projection(
          width, height,
          90,
          0.4f, 50.0f
        )
      }
  };
  hud_camera = new gl::camera{
    gl::view(
      math::vector_3d(0, 0, 5.0), math::quaternion(0, math::vector_3d(1.0f, 0.0f, 0.0f)),
      math::vector_3d(0, 0, 0), math::quaternion(0, 0, 0, 0)
    ),
    gl::orthogonal_projection(
      width, height,
      -1.0f, 1.0f,
      -1.0f, 1.0f,
      0.1f, 100.0f
    )
  };

  current_camera = 0;

  update_hud();

  gl::bltz::save(*scene_graph, "start.bltz");

  engine->start();

  return (0);
}