#include "session.hpp"
#include "color.hpp"
#include <iostream>

Paddle::Paddle(float scr_width, float scr_height, float w, float h) {
  this->c = color::purple;
  this->rect = {
    scr_width / 2 - w / 2,
    scr_height - h - CHANGE_ME_PADDING,
    w, h
  };
};

Ball::Ball(float r, float x, float y, float dx_dt, float dy_dt, Color c) {
  this->r = r;
  this->c = c;
  this->self_destructing = false;
  this->bounces = 3;
  this->p.x = x; this->p.y = y;
  this->v.x = dx_dt;
  this->v.y= dy_dt;
  TraceLog(LOG_INFO, "Created ball @ (%f, %f).\n", this->p.x, this->p.y);
}
Ball::~Ball(void) {
  TraceLog(LOG_INFO, "Deleted ball.\n");
}

void Session::new_ball(float r, float x, float y,
                       float dx_dt, float dy_dt, Color c)
{
  this->ball = new Ball(r, x, y, dx_dt, dy_dt, c);
};
void Session::self_destruct_sequence(void) {
  this->ball->self_destructing = true;
}
void Session::new_paddle(float w, float h) {
  this->paddle = new Paddle((float)this->width, (float)this->height, w, h);
};
void Session::translate_paddle(float dx) { this->paddle->rect.x += dx; };
void Session::toggle_pause(void) { this->pause = !this->pause; } 
Session::Session(int w, int h, const char *title) {
  InitWindow(w, h, title);
  if(!IsWindowReady()) exit(EXIT_FAILURE);
  this->pause = true;
  this->width  = w;
  this->height = h;
  this->dt = 1;
  this->origin = {0, 0};
  this->center = {(float) w / 2, (float) h / 2};
  this->new_ball(15, this->center.x, this->center.y - 100, 0, 0, color::aqua);
  this->new_paddle(150, 20);
};
Session::~Session(void) {
  std::cout << "Tearing down session...\n";
  if (this->ball != nullptr) delete this->ball;
  delete this->paddle;
  CloseWindow();
  exit(EXIT_SUCCESS);
};
