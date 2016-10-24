#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {                // total song in bytes = 35
                                // setup bytes 11
  0x03,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x09, 0x00,                   // Address of track 2
  0x01,                         // Channel 0 entry track (PULSE)
  0x00,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x00,                         // Channel 3 entry track (NOISE)

  //"Track 0"                   // ticks = 0, bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 1"                   // ticks = 1216, bytes = 6
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0xFD, 18, 2,                  // REPEAT: count = 19 - track = 2 (19 * 64 ticks)
  0xFE,                         // RETURN

  //"Track 2"                   // ticks = 64, bytes = 15
  0x00 +  1,                    // NOTE ON: note = 1 (delay 1 tick)
  0x9F +  4,                    // DELAY: ticks = 4
  0x00,                         // NOTE OFF (delay 1 tick)
  0x9F +  4,                    // DELAY: ticks = 4
  0x00 +  5,                    // NOTE ON: note = 5 (delay 1 tick)
  0x9F +  4,                    // DELAY: ticks = 4
  0x00,                         // NOTE OFF (delay 1 tick)
  0x9F +  4,                    // DELAY: ticks = 4
  0x00 +  8,                    // NOTE ON: note = 8 (delay 1 tick)
  0x9F +  4,                    // DELAY: ticks = 4
  0x00,                         // NOTE OFF (delay 1 tick)
  0x9F +  38,                   // DELAY: ticks = 38
  0x4B, 3,                      // FX: ADD TRANSPOSITION: notes = 3
  0xFE,                         // RETURN
};

#endif
