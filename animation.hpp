#ifndef PPONG_ANIM_H
#define PPONG_ANIM_H
#include <raylib.h>
#include <cstddef>
#include <memory>
#include "session.hpp"

struct FrameBuffer {
  size_t num_frames;
  std::unique_ptr<Texture2D[]> frames;
};

namespace animation {
  FrameBuffer explosion;
  void load_explosion(Session &s) {
    explosion.num_frames = 30;
    explosion.frames = std::make_unique<Texture2D[]>(explosion.num_frames);
    static unsigned int color = 0xFF0000FF;
    for (size_t i = 0; i < explosion.num_frames; i++) {
      RenderTexture2D target = LoadRenderTexture(s.width, s.height);
      BeginTextureMode(target);
        ClearBackground(BLANK);
        DrawCircleV(s.center, 5 + ((float)i * 5), GetColor(color));
      EndTextureMode();
      explosion.frames[i] = target.texture;
      color -= 0x00000005;
      color += 0x00111100;
    }
  }
  void req_frame_at(Vector2 pos, FrameBuffer &bfr, size_t fnum) {
    DrawTextureV(bfr.frames[fnum], pos, WHITE);
  }
}


#endif // PPONG_ANIM_H
