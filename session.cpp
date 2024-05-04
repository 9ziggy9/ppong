#include "session.hpp"
#include "color.hpp"
#include <iostream>
#include "sound.hpp"

namespace effect {
  struct Explosion {
    Vector2 position; float radius;
    float t_left; float rad_rate;
    unsigned int color;

    Explosion(Vector2 p, float r)
      : position(p), radius(r),
        t_left(0.21f), rad_rate(1.5f),
        color(0xFF0000FF) {}

    void update(float dt) {
      if (t_left > 0) {
        radius += rad_rate;
        color -= 0x00000005;
        t_left -= dt;
      }
    }

    void draw() const {
      if (t_left > 0) DrawCircleV(position, radius, GetColor(color));
    }

    bool is_finished() const { return t_left <= 0; }
  };
}

Paddle::Paddle(float scr_width, float scr_height, float w, float h) {
  this->c = color::purple;
  this->rect = {
    scr_width / 2 - w / 2,
    scr_height - h - CHANGE_ME_PADDING,
    w, h
  };
}

Ball::Ball(float r, float x, float y, float dx_dt, float dy_dt, Color c) {
  this->r = r;
  this->c = c;
  this->self_destructing = false;
  this->countdown = 1.5f;
  this->acc       = 0.0f;
  this->bounces = 3;
  this->p.x = x; this->p.y = y;
  this->v.x = dx_dt;
  this->v.y= dy_dt;
  TraceLog(LOG_INFO, "Created ball @ (%f, %f).\n", this->p.x, this->p.y);
}
Ball::~Ball(void) {
  TraceLog(LOG_INFO, "Deleted ball.\n");
}

void Session::new_ball(float r,
                       float x, float y,
                       float dx_dt, float dy_dt, Color c)
{
  this->balls.emplace_back(Ball(r, x, y, dx_dt, dy_dt, c));
}
void Session::self_destruct_sequence(std::vector<Ball>::iterator it) {
  it->self_destructing = true;
}
void Session::new_paddle(float w, float h) {
  this->paddle = new Paddle((float)this->width, (float)this->height, w, h);
}
void Session::translate_paddle(float dx) { this->paddle->rect.x += dx; };
void Session::toggle_pause(void) {
  this->md = (this->md == mode::PAUSED)
    ? mode::RUNNING
    : mode::PAUSED;
} 

Session::Session(int w, int h, const char *title) {
  InitWindow(w, h, title);
  if(!IsWindowReady()) exit(EXIT_FAILURE);
  this->md     = mode::PAUSED;
  this->width  = w;
  this->height = h;
  this->dt     = 1;
  this->origin = {0, 0};
  this->center = {(float) w / 2, (float) h / 2};
  this->new_ball(15, this->center.x, this->center.y - 100, 0, 0, color::aqua);
  this->new_paddle(150, 20);
};
Session::~Session(void) {
  std::cout << "Tearing down session...\n";
  delete this->paddle;
  CloseWindow();
  exit(EXIT_SUCCESS);
};
