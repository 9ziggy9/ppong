#ifndef PPONG_RENDER_H_
#define PPONG_RENDER_H_
#include <raylib.h>
#include <raymath.h>

#include "session.hpp"
#include "color.hpp"
#include "txt.hpp"
#include "sound.hpp"

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

  inline void balls(Session &s) {
    for (auto b = s.balls.begin(); b != s.balls.end();) {
      if (b->self_destructing) {
        const float bp = 0.16f;
        b->acc += s.dt;
        b->countdown -= s.dt;
        if (b->countdown <= 0.0f) {
          sound::play_sound(sound::sound_beep);
          s.expls.emplace_back(b->p, b->r);
          b = s.balls.erase(b);
          continue;
        } else { // TODO: generalized flashing function
          if (b->acc <= 0.33f * bp)
            b->c = color::red;
          else if ((b->acc > 0.33f * bp) && (b->acc <= 0.66f * bp))
            b->c = color::yellow;
          else if ((b->acc > 0.66f * bp) && (b->acc < 1.0f * bp))
            b->c = color::dark_brown;
          else
            b->acc = 0.0f;
        }
      }
      DrawCircle((int)b->p.x, (int)b->p.y, b->r, b->c);
      ++b;
    }
  }

  inline void splodies(Session &s) {
    for (auto it = s.expls.begin(); it != s.expls.end();) {
      it->update(s.dt);
      it->draw();
      if (it->is_finished()) it = s.expls.erase(it);
      else ++it;
    }
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
