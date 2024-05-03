#ifndef PPONG_TEXT_H_
#define PPONG_TEXT_H_
#include "session.hpp"
#include <iostream>

namespace txt {
  inline int font_sz_dialog = 80;
  inline int font_sz_debug  = 16;
  inline const char *paused     = "PAUSED";
  inline const char *gameover   = "GAME OVER";
  inline const char *restart    = "(space to play again)";
  inline void debug(Session &s) {
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

#endif // PPONG_TEXT_H_
