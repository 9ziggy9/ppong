#include <iostream>
#include <raylib.h>
#include <assert.h>


#define CHANGE_ME_PADDING 10.0f
struct Paddle {
  Color c;
  Rectangle rect;
  Paddle(float scr_width, float scr_height, float w, float h) {
    this->c = BLUE;
    this->rect = {
      scr_width / 2,
      scr_height - h - CHANGE_ME_PADDING,
      w, h
    };
  };
};

struct Ball {
  float r; float x; float y;
  float dx_dt; float dy_dt;
  Color c;
  Ball(float r, float x, float y, float dx_dt, float dy_dt, Color c) {
    this->r = r;
    this->x = x; this->y = y;
    this->dx_dt = dx_dt;
    this->dy_dt = dy_dt;
    TraceLog(LOG_INFO, "Created ball @ (%f, %f).\n", this->x, this->y);
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

  Paddle *paddle;
  void new_paddle(float w, float h) {
    this->paddle = new Paddle(this->width, this->height, w, h);
  };

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
    this->new_ball(30, this->center.x, this->center.y,
                   0, 0, RED);
    this->new_paddle(150, 20);
  };
  ~Session() {
    std::cout << "Tearing down session...\n";
    CloseWindow();
    exit(EXIT_SUCCESS);
  };
};


// confur

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
    DrawCircle(s.ball->x, s.ball->y, s.ball->r, RED);
  }
  void paddle(Session &s) {
    DrawRectangleRec(s.paddle->rect, s.paddle->c);
  }
}

namespace physics {
  static float g = 1.0f;
  void apply_gravity(Session &s) {
    s.ball->dy_dt += g * (s.dt);
    s.ball->y     += s.ball->dy_dt * (s.dt);
  }
}

void handle_keys(Session *s) {
  int k = GetKeyPressed();
  switch (k) {
  case KEY_ESCAPE: case 'Q': delete s;
  case KEY_SPACE: s->toggle_pause();
  default: break;
  }
}

int main(int argc, char **argv) {
  assert(argc == 2 && "[USAGE]: provide fps bruh\n");

  Session *session = new Session(640, 480, "ppong");
  render::load_bg_texture(session->width,
                          session->height, 16);

  SetTargetFPS(atoi(argv[1]));

  while (!WindowShouldClose()) {
    handle_keys(session);
    BeginDrawing();
    if (!session->pause) {
          render::bg(*session, DARKGRAY);
          render::ball(*session);
          render::paddle(*session);
          physics::apply_gravity(*session);
    } else {
      ClearBackground(BLUE);
    }
    EndDrawing();
  }

  return EXIT_SUCCESS; // unreachable
}
