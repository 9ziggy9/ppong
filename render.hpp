#ifndef PPONG_RENDER_H_
#define PPONG_RENDER_H_
#include "session.hpp"
#include "color.hpp"

namespace render {
  inline RenderTexture2D bg_texture;
  inline void load_bg_texture(int w, int h, int sz) {
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
  inline void bg(Session &s, Color tint) {
    DrawTextureV(render::bg_texture.texture, s.origin, tint);
  }
  inline void ball(Session &s) {
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
  inline void paddle(Session &s) {
    DrawRectangleRec(s.paddle->rect, s.paddle->c);
  }
}

#endif // PPONG_RENDER_H_
