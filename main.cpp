#include <raylib.h>
#include <assert.h>
#include "txt.hpp"
#include "session.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "sound.hpp"

void handle_keys(Session *);

int main(int argc, char **argv) {
  assert(argc == 2 && "[USAGE]: provide fps bruh\n");

  Session *session = new Session(960, 720, "ppong");
  session->load_next_level();

  render::load_bg_texture(session->width,
                          session->height, 16);

  SetTargetFPS(atoi(argv[1]));
  Font font_main = LoadFont("resources/font/setbackt.ttf");
  sound::init();

  while (!WindowShouldClose()) {
    handle_keys(session);
    BeginDrawing();
      switch(session->md) {
      case mode::RUNNING:
        sound::loop_song(sound::music_main);
        render::bg(*session, DARKGRAY);
        render::balls(*session);
        render::paddle(*session);
        render::splodies(*session);
        physics::time_step(*session);
        physics::update(*session);
        txt::debug(*session);
        break;
      case mode::PAUSED:
        render::menu_pause(*session, font_main);
        break;
      case mode::GAMEOVER: {
        ClearBackground(BLACK);
        render::menu_over(*session, font_main);
        sound::play_once(sound::sound_over, false);
      }
      }
    EndDrawing();
  }
  return EXIT_SUCCESS; // unreachable
}

void handle_keys(Session *s) {
  static const float dx_dt = 400.0f;
  const float dx = dx_dt * GetFrameTime();
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
