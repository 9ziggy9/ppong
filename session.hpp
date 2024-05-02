#ifndef PPONG_SESSION_H_
#define PPONG_SESSION_H_
#include <raylib.h>
#include <cstdint>

#define CHANGE_ME_PADDING 10.0f

struct Paddle {
  Color c; Rectangle rect;
  Paddle(float, float, float, float);
};

struct Ball {
  float r; Vector2 p; Vector2 v;
  Color c; uint8_t bounces;
  bool self_destructing;
  Ball(float, float, float, float, float, Color); ~Ball(void);
};

struct Session {
  int width; int height;
  bool pause;
  Vector2 origin;
  Vector2 center;
  float dt;
  Ball *ball;
  void new_ball(float, float, float, float, float, Color);
  void self_destruct_sequence(void);
  Paddle *paddle;
  void new_paddle(float, float);
  void translate_paddle(float);
  void toggle_pause(void);
  Session(int w, int h, const char *title); ~Session(void);
};

#endif // PPONG_SESSION_H_
