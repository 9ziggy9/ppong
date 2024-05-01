#include <cstdint>
#include <iostream>
#include <raylib.h>
#include <assert.h>
#include "color.h"

#define CHANGE_ME_PADDING 10.0f
struct Paddle {
  Color c;
  Rectangle rect;
  Paddle(float scr_width, float scr_height, float w, float h) {
    this->c = color::purple;
    this->rect = {
      scr_width / 2 - w / 2,
      scr_height - h - CHANGE_ME_PADDING,
      w, h
    };
  };
};

struct Ball {
  float r;
  Vector2 p;
  Vector2 v;
  Color c;
  uint8_t bounces;
  bool self_destructing;
  Ball(float r, float x, float y, float dx_dt, float dy_dt, Color c) {
    this->r = r;
    this->c = c;
    this->self_destructing = false;
    this->bounces = 3;
    this->p.x = x; this->p.y = y;
    this->v.x = dx_dt;
    this->v.y= dy_dt;
    TraceLog(LOG_INFO, "Created ball @ (%f, %f).\n", this->p.x, this->p.y);
  }
  ~Ball(void) {
    TraceLog(LOG_INFO, "Deleted ball.\n");
  }
};

struct Session {
  int width; int height;
  bool pause;
  Vector2 origin;
  Vector2 center;
  float dt;

  Ball *ball;
  void new_ball(float r, float x, float y, float dx_dt, float dy_dt, Color c) {
    this->ball = new Ball(r, x, y, dx_dt, dy_dt, c);
  };
  void self_destruct_sequence(void) { this->ball->self_destructing = true; }

  Paddle *paddle;
  void new_paddle(float w, float h) {
    this->paddle = new Paddle((float)this->width, (float)this->height, w, h);
  };
  void translate_paddle(float dx) { this->paddle->rect.x += dx; };

  void toggle_pause(void) { this->pause = !this->pause; } 
  Session(int w, int h, const char *title) {
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
  ~Session() {
    std::cout << "Tearing down session...\n";
    if (this->ball != nullptr) delete this->ball;
    delete this->paddle;
    CloseWindow();
    exit(EXIT_SUCCESS);
  };
};

namespace render {
  static RenderTexture2D bg_texture;
  void load_bg_texture(int w, int h, int sz) {
    bg_texture = LoadRenderTexture(w, h);
    BeginTextureMode(bg_texture);
    for (int y = 0; y < h / sz; y++) {
      for (int x = 0; x < w / sz; x++) {
        DrawRectangle(x * sz, y * sz, sz, sz,
                      (x + y) % 2 ? DARKGRAY : GRAY);
      }
    }
    EndTextureMode();
  }
  void bg(Session &s, Color tint) {
    DrawTextureV(render::bg_texture.texture, s.origin, tint);
  }
  void ball(Session &s) {
    if (s.ball == nullptr) return;
    if (s.ball->self_destructing) {
      static float acc        = 0.0f;
      const float color_break = 4.0f;
      acc += s.dt;
      if (acc >= color_break) {
        acc = 0;
        s.ball->c = color::red;
      }
      if (acc == 0) {
        s.ball->c = color::yellow;
      }
    }
    DrawCircle((int)s.ball->p.x, (int)s.ball->p.y, s.ball->r, s.ball->c);
  }
  void paddle(Session &s) {
    DrawRectangleRec(s.paddle->rect, s.paddle->c);
  }
}

namespace physics {
  static float g = 1000.0f;
  void time_step(Session &s) { s.dt = GetFrameTime(); }
  void apply_gravity(Session &s, float step) {
    s.ball->p.y += s.ball->v.y * (step) + (0.5) * g * (step * step);
    s.ball->v.y += g * (step);
  }
  void apply_rebound(Session &s) {
    if (s.ball == nullptr) return;
    if (CheckCollisionCircleRec(s.ball->p, s.ball->r, s.paddle->rect)) {
      s.ball->v.y *= -1;
      s.ball->v.x = 4.0 * (s.ball->p.x -
                           s.paddle->rect.x -
                           s.paddle->rect.width/2);
      if (s.ball->p.y + s.ball->r >= s.paddle->rect.y) {
        s.ball->p.y = s.paddle->rect.y - s.ball->r;
      }
      switch (--s.ball->bounces) {
      case 2: s.ball->c = color::green;   return;
      case 1: s.ball->c = color::orange;  return;
      case 0: s.self_destruct_sequence(); return;
      default:                            return;
      }
    }
  }
  void apply_boundaries(Session &s) {
    if (s.ball == nullptr) return;
    if ((s.ball->p.x + s.ball->r) >= s.width || (s.ball->p.x - s.ball->r) <= 0)
    {
      s.ball->v.x *= -1;
    }
  }
  void update(Session &s) {
    // Semi-fixed timestep for stability
    // Reference: https://gafferongames.com/post/fix_your_timestep/
    static float acc = 0.0f;
    acc += s.dt;
    const float step = 0.005f; // fixed physics time step
    if (s.ball != nullptr) {
      while (acc >= step) {
        apply_gravity(s, step);
        s.ball->p.x += s.ball->v.x * step;
        apply_rebound(s);
        apply_boundaries(s);
        acc -= step;
      }
    }
  }
}

void handle_keys(Session *s) {
  static const float dx = 1.5f;

  if      (IsKeyDown(KEY_LEFT))  s->translate_paddle(-dx);
  else if (IsKeyDown(KEY_RIGHT)) s->translate_paddle(dx);
  else {
    int kp = GetKeyPressed();
    switch (kp) {
    case KEY_SPACE: s->toggle_pause();   break;
    case KEY_ESCAPE: case 'Q': delete s; break;
    }
  }
}

namespace txt {
  int font_sz_dialog = 80;
  int font_sz_debug  = 16;
  const char *paused = "PAUSED";
  void debug(Session &s) {
    char txt_dt[32];
    char txt_fps[32];
    std::sprintf(txt_fps, "fps: %d", GetFPS());
    std::sprintf(txt_dt, "dt: %.4f", s.dt);
    int txt_dt_width  = MeasureText(txt_dt, font_sz_debug) + 4;
    int txt_fps_width = MeasureText(txt_fps, font_sz_debug) + 4;
    DrawText(txt_dt, s.width - txt_dt_width, 1, font_sz_debug, GREEN);
    DrawText(txt_fps, s.width - txt_fps_width,
             1 + font_sz_debug, font_sz_debug, GREEN);
  }
}

int main(int argc, char **argv) {
  assert(argc == 2 && "[USAGE]: provide fps bruh\n");

  Session *session = new Session(960, 720, "ppong");
  render::load_bg_texture(session->width,
                          session->height, 16);

  SetTargetFPS(atoi(argv[1]));

  Font font_main = LoadFont("resources/font/setbackt.ttf");

  while (!WindowShouldClose()) {
    handle_keys(session);
    BeginDrawing();
      if (!session->pause) {
        render::bg(*session, DARKGRAY);
        render::ball(*session);
        render::paddle(*session);

        physics::time_step(*session);
        physics::update(*session);

        txt::debug(*session);
      } else {
        DrawRectangle(0, 0,
                      session->width, session->height,
                      (Color){0, 0, 0, 1}); // overlay
        int txt_width = MeasureText(txt::paused, txt::font_sz_dialog);
        DrawTextEx(font_main, "PAUSED",
                   Vector2{((float)session->width -
                            (float)txt_width) / 2,
                           ((float)session->height -
                            (float)txt::font_sz_dialog) / 2},
                   txt::font_sz_dialog, 10, GREEN);
      }
    EndDrawing();
  }

  return EXIT_SUCCESS; // unreachable
}
