#include <raylib.h>
#include "txt.hpp"
#include "session.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "sound.hpp"
#include "animation.hpp"

#define PPONG_FPS 144
void handle_keys(Session *);

#ifndef _PPONG_DEBUG_
  int main(int argc, char **argv) {
    Session *session = new Session(960, 720, "ppong");
    session->load_next_level();

    render::load_bg_texture(session->width,
                            session->height, 16);

    SetTargetFPS((argc == 2) ? atoi(argv[1]) : PPONG_FPS);

    Font font_main = LoadFont("resources/font/setbackt.ttf");

    sound::init();
    animation::load_explosion(*session);

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
        }
        }
      EndDrawing();
    }
    return EXIT_SUCCESS; // unreachable
  }
#else
  int main(int argc, char **argv) {
    Session *session = new Session(960, 720, "ppong");
    session->load_next_level();

    render::load_bg_texture(session->width,
                            session->height, 16);

    SetTargetFPS((argc == 2) ? atoi(argv[1]) : PPONG_FPS);

    sound::init();

    animation::load_explosion(*session);

    uint32_t f = 0;
    while (!WindowShouldClose()) {
      BeginDrawing();
        render::bg(*session, DARKGRAY);
        animation::req_frame_at(Vector2{-100,-100}, animation::explosion, f++);
      EndDrawing();
    }
    return EXIT_SUCCESS; // unreachable
  }
#endif

void handle_keys(Session *s) {
  static const float dx_dt = 750.0f;
  const float dx = dx_dt * GetFrameTime();
  if      (IsKeyDown(KEY_LEFT))  s->translate_paddle(-dx);
  else if (IsKeyDown(KEY_RIGHT)) s->translate_paddle(dx);
  else {
    int kp = GetKeyPressed();
    switch (kp) {
    case KEY_SPACE: {
      if (s->md == mode::GAMEOVER) {
        s->reset();
        s->load_next_level();
      } else {
        s->toggle_pause();
      }
    } break;
    case KEY_ESCAPE: case 'Q': delete s; break;
    }
  }
}
