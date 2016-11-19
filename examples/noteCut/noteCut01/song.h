#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {                // total song in bytes = 76
  //                            // setup bytes 21
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

  //"Track 1"                   // ticks = 2048, bytes = 6
  0x40, 16,                     // FX: SET VOLUME: volume = 63
  0xFD, 31, 2,                  // REPEAT: count = 32 - track = 2 (32 * 64 ticks)
  0xFE,                         // RETURN

  //"Track 2"                   // ticks = 64, bytes = 9
  0x00 + 36,                    // NOTE ON: note = 36 (delay 1 tick)
  0x52, 1,                      // FX: NOTE CUT: amount of ticks between switching note ON and note OFF
  0x9F + 63,                    // DELAY: 63 ticks
  0x00 + 39,                    // NOTE ON: note = 39 (delay 1 tick)
  0x9F + 63,                    // DELAY: 63 ticks
  0x00 + 41,                    // NOTE ON: note = 41 (delay 1 tick)
  0x9F + 63,                    // DELAY: 63 ticks
  0x00 + 36,                    // NOTE ON: note = 36 (delay 1 tick)
  0x9F + 63,                    // DELAY: 63 ticks
  0xFE,                         // RETURN

};

#endif
