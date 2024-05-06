#ifndef PPONG_PHYSICS_H_
#define PPONG_PHYSICS_H_
#include "session.hpp"
#include "color.hpp"
#include "sound.hpp"

namespace physics {
  inline float g = 1000.0f;

  inline void time_step(Session &s) { s.dt = GetFrameTime(); }

  inline void apply_gravity(Session &s, float step) {
    for (auto &b : s.balls) {
      b.p.y += b.v.y * (step) + (0.5f) * g * (step * step);
      b.v.y += g * (step);
    }
  }

  inline void apply_rebound(Session &s) {
    for (auto it = s.balls.begin(); it != s.balls.end(); ++it)
      if (CheckCollisionCircleRec(it->p, it->r, s.paddle->rect)) {
        sound::play_sound(sound::sound_beep);
        it->v.y *= -1;
        it->v.x = 4.0f*(it->p.x - s.paddle->rect.x - s.paddle->rect.width / 2);
        if (it->p.y + it->r >= s.paddle->rect.y) {
          it->p.y = s.paddle->rect.y - it->r;
        }
        switch (--it->bounces) {
        case 2: it->c = color::green;         break;
        case 1: it->c = color::orange;        break;
        case 0: s.self_destruct_sequence(it); break;
        default:                              break;
      }
    }
  }

  inline void apply_boundaries(Session &s) {
    for (auto &b : s.balls) {
      if ((b.p.x + b.r) >= (float) s.width || (b.p.x - b.r) <= 0)
        b.v.x *= -1;
      if ((b.p.y + b.r) >= (float) s.height)
        s.md = mode::GAMEOVER;
      else if ((b.p.y - b.r) <= 0)
        b.v.y *= -1;
    }
  }

  inline void update(Session &s) {
    // Semi-fixed timestep for stability
    // Reference: https://gafferongames.com/post/fix_your_timestep/
    static float acc = 0.0f;
    acc += s.dt;
    const float step = 0.005f; // fixed physics time step
    for (auto &b : s.balls) {
      while (acc >= step) {
        apply_gravity(s, step);
        b.p.x += b.v.x * step;
        apply_rebound(s);
        apply_boundaries(s);
        acc -= step;
      }
    }
    if (s.balls.size() == 0) s.load_next_level();
  }
}

#endif // PPONG_PHYSICS_H_
