#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {                // total song in bytes = 50
  //                            // setup bytes 13
  0x07,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x0B, 0x00,                   // Address of track 2
  0x1C, 0x00,                   // Address of track 3
  0x23, 0x00,                   // Address of track 4
  0x2A, 0x00,                   // Address of track 5
  0x31, 0x00,                   // Address of track 6
  0x00,                         // Channel 0 entry track (PULSE)
  0x01,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x00,                         // Channel 3 entry track (NOISE)

  //"Track 0"                   // ticks = 0 / bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 1"                   // ticks = 2048 / bytes = 8
  0x9D, 32,                     // SET song tempo: value = 25
  0x40, 48,                     // FX: SET VOLUME: volume = 48
  0xFD, 9, 2,                   // REPEAT: count = 7 + 1 / track = 2  (8 * 256 ticks)
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 2"                   // ticks = 256 / bytes = 18
  0x41, -2,                     // FX: VOLUME SLIDE ON: steps = -2
  0x9C, 8,                      // ADD song tempo: value = 2
  0xFD, 3, 3,                   // REPEAT: count = 3 + 1 / track = 3  (8 * 256 ticks)
  0xFD, 3, 4,                   // REPEAT: count = 3 + 1 / track = 3  (8 * 256 ticks)
  0xFD, 3, 5,                   // REPEAT: count = 3 + 1 / track = 3  (8 * 256 ticks)
  0xFD, 3, 6,                   // REPEAT: count = 3 + 1 / track = 3  (8 * 256 ticks)
  0xFE,                         // RETURN

  //"Track 3"                   // ticks = 64 / bytes = 7
  0x00 + 37,                    // NOTE ON: note = 37
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 40,                    // NOTE ON: note = 41
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 44,                    // NOTE ON: note = 44
  0x9F + 16,                    // DELAY: ticks = 21
  0xFE,                         // RETURN

  //"Track 4"                   // ticks = 64 / bytes = 7
  0x00 + 37,                    // NOTE ON: note = 37
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 40,                    // NOTE ON: note = 41
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 45,                    // NOTE ON: note = 44
  0x9F + 16,                    // DELAY: ticks = 21
  0xFE,                         // RETURN

  //"Track 5"                   // ticks = 64 / bytes = 7
  0x00 + 37,                    // NOTE ON: note = 37
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 42,                    // NOTE ON: note = 41
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 46,                    // NOTE ON: note = 44
  0x9F + 16,                    // DELAY: ticks = 21
  0xFE,                         // RETURN

  //"Track 6"                   // ticks = 64 / bytes = 7
  0x00 + 37,                    // NOTE ON: note = 37
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 42,                    // NOTE ON: note = 41
  0x9F + 16,                    // DELAY: ticks = 21
  0x00 + 47,                    // NOTE ON: note = 44
  0x9F + 16,                    // DELAY: ticks = 21
  0xFE,                         // RETURN

};

#endif
