#ifndef PPONG_RENDER_H_
#define PPONG_RENDER_H_
#include "session.hpp"
#include "color.hpp"
#include "txt.hpp"
#include <raylib.h>
#include <raymath.h>

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
      static float countdown  = 3.0f;
      static float acc        = 0.0f;
      const float bp          = 0.16f;
      acc += s.dt;
      countdown -= s.dt;
      if (countdown <= 0.0f) s.splode();
      else {
        // TODO: generalized flashing function
        if (acc <= 0.33f * bp)
          s.ball->c = color::red;
        else if ((acc > 0.33f * bp) && (acc <= 0.66f * bp))
          s.ball->c = color::yellow;
        else if ((acc > 0.66f * bp) && (acc < 1.0f * bp))
          s.ball->c = color::dark_brown;
        else
          acc = 0.0f;
      }
    }
    DrawCircle((int)s.ball->p.x, (int)s.ball->p.y, s.ball->r, s.ball->c);
  }
  inline void paddle(Session &s) {
    DrawRectangleRec(s.paddle->rect, s.paddle->c);
  }
  inline void menu_pause(Session &s, Font font) {
    DrawRectangle(0, 0, s.width, s.height, Color{0, 0, 0, 1});
    int txt_width = MeasureText(txt::paused, txt::font_sz_dialog);
    DrawTextEx(font, "PAUSED",
               Vector2{((float)s.width -
                        (float)txt_width) / 2,
                       ((float)s.height -
                        (float)txt::font_sz_dialog) / 2},
               (float)txt::font_sz_dialog, 10, GREEN);
    
  }
  inline void menu_over(Session &session, Font font) {
    DrawRectangle(0, 0, session.width, session.height, Color{0, 0, 0, 1});
    int txt_go_width  = MeasureText(txt::gameover, txt::font_sz_dialog);
    int txt_rst_width = MeasureText(txt::restart, txt::font_sz_debug);
    Vector2 center_screen = Vector2{((float)session.width -
                                     (float)txt_go_width) / 2,
                                    ((float)session.height -
                                     (float)txt::font_sz_dialog) / 2};
    DrawTextEx(font, txt::gameover, center_screen,
               (float)txt::font_sz_dialog, 10, RED);
    DrawTextEx(font, txt::restart,
               Vector2Add(center_screen, Vector2{(float)txt_rst_width / 2,
                                                 (float)txt::font_sz_dialog}),
               (float)txt::font_sz_debug, 10, GREEN);
  }
}

#endif // PPONG_RENDER_H_
