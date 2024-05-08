#include <iostream>
#include <random>

#include "session.hpp"
#include "color.hpp"
#include "sound.hpp"

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
  this->bounces   = 3;
  this->p.x = x; this->p.y = y;
  this->v.x = dx_dt;
  this->v.y = dy_dt;
  TraceLog(LOG_INFO, "Created ball @ (%f, %f).\n", this->p.x, this->p.y);
}
Ball::~Ball(void) {}

void Session::new_ball(float r,
                       float x, float y,
                       float dx_dt, float dy_dt, Color c)
{
  this->balls.emplace_back(Ball(r, x, y, dx_dt, dy_dt, c));
}
void Session::self_destruct_sequence(std::vector<Ball>::iterator it) {
  it->self_destructing = true;
}
void Session::new_paddles(float w, float h) {
  this->paddle_r = new Paddle((float)this->width * 1.5f,
                              (float)this->height, w, h);
  this->paddle_l = new Paddle((float)this->width * 0.5f,
                              (float)this->height, w, h);
}
void Session::translate_paddle(float dx, Paddle *p) { p->rect.x += dx; };
void Session::toggle_pause(void) {
  this->md = (this->md == mode::PAUSED)
    ? mode::RUNNING
    : mode::PAUSED;
} 

#define MIN_RADIUS  5 
#define MAX_RADIUS 30 
#define X_REGION   50

void Session::load_next_level(void) {
  int balls_left = ++this->lvl;
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<> x_dist((float)(this->width) / 2 - X_REGION,
                                          (float)(this->width) / 2 + X_REGION);
  std::uniform_real_distribution<> y_dist((float)(this->height) / 8,
                                          (float)(this->height) / 4);
  std::uniform_real_distribution<> radius_dist(MIN_RADIUS, MAX_RADIUS);

  while (--balls_left >= 0)
    this->new_ball((float)radius_dist(gen),
                   (float)x_dist(gen),
                   (float)y_dist(gen),
                   0, 0, color::aqua); 
}

void Session::reset(void) {
  delete this->paddle_r;
  delete this->paddle_l;
  this->balls.clear();
  this->new_paddles(150,20);
  SeekMusicStream(sound::music_main, 0.0f);
  this->lvl = 0;
  this->md = mode::RUNNING;
}

Session::Session(int w, int h, const char *title) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(w, h, title);
  if (!IsWindowReady()) exit(EXIT_FAILURE);
  this->md     = mode::PAUSED;
  this->lvl    = 0;
  this->width  = w;
  this->height = h;
  this->dt     = 1;
  this->origin = {0, 0};
  this->center = {(float) w / 2, (float) h / 2};
  this->new_paddles(150, 20);
};

Session::~Session(void) {
  delete this->paddle_r;
  delete this->paddle_l;
  CloseWindow();
  exit(EXIT_SUCCESS);
};
