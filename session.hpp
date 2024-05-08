#ifndef PPONG_SESSION_H_
#define PPONG_SESSION_H_
#include <raylib.h>
#include <cstdint>
#include <vector>

#include "effects.hpp"

#define CHANGE_ME_PADDING 10.0f

struct Paddle {
  Color c; Rectangle rect;
  Paddle(float, float, float, float);
};

struct Ball {
  float r; Vector2 p; Vector2 v;
  Color c; uint8_t bounces;
  bool self_destructing;
  float countdown;
  float acc;
  Ball(float, float, float, float, float, Color);
  ~Ball(void);
};

enum struct mode {
  RUNNING,
  PAUSED,
  GAMEOVER,
};

struct Session {
  int width; int height;
  mode md;
  uint8_t lvl;
  Vector2 origin;
  Vector2 center;
  float dt;
  std::vector<Ball> balls;
  std::vector<effects::Explosion> expls;
  void new_ball(float, float, float, float, float, Color);
  void load_next_level(void);
  void self_destruct_sequence(std::vector<Ball>::iterator);
  Paddle *paddle_r;
  Paddle *paddle_l;
  void new_paddles(float, float);
  void translate_paddle(float, Paddle *);
  void toggle_pause(void);
  Session(int w, int h, const char *title);
  void reset(void);
  ~Session(void);
};

#endif // PPONG_SESSION_H_
