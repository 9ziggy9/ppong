#ifndef PPONG_EFFECTS_H
#define PPONG_EFFECTS_H
#include <raylib.h>
namespace effects {
  struct Explosion {
    Vector2 position; float radius;
    float t_left; float rad_rate;
    unsigned int color;

    Explosion(Vector2 p, float r)
      : position(p), radius(r),
        t_left(0.21f), rad_rate(1.5f),
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
#endif // PPONG_EFFECTS_H
