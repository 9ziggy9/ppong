#ifndef PPONG_SOUND_H
#define PPONG_SOUND_H
#include "raylib.h"

namespace sound {
  inline Sound sound_over;
  inline Sound sound_beep;
  inline Sound sound_splode;
  inline Music music_main;
  inline void init(void) {
    InitAudioDevice();
    sound_splode = LoadSound("resources/sound/splode.wav");
    sound_over   = LoadSound("resources/sound/over.wav");
    sound_beep   = LoadSound("resources/sound/beep.mp3");
    music_main   = LoadMusicStream("resources/sound/main.wav");
  }
  inline void play_sound(Sound snd) { PlaySound(snd); }
  inline void play_once(Sound snd, bool again) {
    static bool played_sound = false;
    if (again) played_sound = false;
    if (played_sound) return; 
    PlaySound(snd);
    played_sound = true;
  }
  inline void loop_song(Music msc) {
    UpdateMusicStream(msc);
    if (!IsMusicStreamPlaying(msc)) PlayMusicStream(msc);
  }
}

#endif // PPONG_SOUND_H
