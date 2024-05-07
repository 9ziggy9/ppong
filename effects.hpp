#ifndef PPONG_EFFECTS_H
#define PPONG_EFFECTS_H
#include <raylib.h>
#include <cstddef>

#if 1
namespace effects {
  struct Explosion {
    Vector2 position;
    float radius;
    size_t frame_count;
    float countdown;
    float acc;
    Explosion(Vector2 p, float r)
      : position(p), radius(r),
        frame_count(0), countdown(2.0f),
        acc(0.0f) {}
  };
}
#endif

// procedural version
#if 0
namespace effects {
  struct Explosion {
    Vector2 position;
    float radius;
    float t_left;
    float rad_rate;
    unsigned int color;

    Explosion(Vector2 p, float r) : position(p),
                                    radius(r),
                                    t_left(0.21f),
                                    rad_rate(1.5f),
                                    color(0xFF0000FF) {}
    void update(float dt) {
      if (t_left > 0) {
        radius += rad_rate;
        color -= 0x00000005;
        t_left -= dt;
      }
    }
    void draw() const {
      if (t_left > 0) DrawCircleV(position, radius, GetColor(color));
    }
    bool is_finished() const { return t_left <= 0; }
  };
}
#endif

#endif // PPONG_EFFECTS_H
