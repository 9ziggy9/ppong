#ifndef PPONG_PHYSICS_H_
#define PPONG_PHYSICS_H_
#include "session.hpp"
#include "color.hpp"

namespace physics {
  inline float g = 1000.0f;
  inline void time_step(Session &s) { s.dt = GetFrameTime(); }
  inline void apply_gravity(Session &s, float step) {
    s.ball->p.y += s.ball->v.y * (step) + (0.5f) * g * (step * step);
    s.ball->v.y += g * (step);
  }
  inline void apply_rebound(Session &s) {
    if (s.ball == nullptr) return;
    if (CheckCollisionCircleRec(s.ball->p, s.ball->r, s.paddle->rect)) {
      s.ball->v.y *= -1;
      s.ball->v.x = 4.0f * (s.ball->p.x -
                           s.paddle->rect.x -
                           s.paddle->rect.width / 2);
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
  inline void apply_boundaries(Session &s) {
    if (s.ball == nullptr) return;
    if ((s.ball->p.x + s.ball->r) >= (float) s.width ||
        (s.ball->p.x - s.ball->r) <= 0)
    {
      s.ball->v.x *= -1;
    }
  }
  inline void update(Session &s) {
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

#endif // PPONG_PHYSICS_H_
