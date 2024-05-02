#include <raylib.h>
#include <assert.h>
#include "txt.hpp"
#include "session.hpp"
#include "physics.hpp"
#include "render.hpp"

void handle_keys(Session *);

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
        DrawRectangle(0, 0, session->width, session->height, Color{0, 0, 0, 1});
        int txt_width = MeasureText(txt::paused, txt::font_sz_dialog);
        DrawTextEx(font_main, "PAUSED",
                   Vector2{((float)session->width -
                            (float)txt_width) / 2,
                           ((float)session->height -
                            (float)txt::font_sz_dialog) / 2},
                   (float)txt::font_sz_dialog, 10, GREEN);
      }
    EndDrawing();
  }

  return EXIT_SUCCESS; // unreachable
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
