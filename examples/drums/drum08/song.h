#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {                // total song in bytes = 76
  //                            // setup bytes 21
  0x08,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x09, 0x00,                   // Address of track 2
  0x12, 0x00,                   // Address of track 3
  0x1B, 0x00,                   // Address of track 4
  0x21, 0x00,                   // Address of track 5
  0x28, 0x00,                   // Address of track 6
  0x2F, 0x00,                   // Address of track 7
  0x01,                         // Channel 0 entry track (PULSE)
  0x00,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x04,                         // Channel 3 entry track (NOISE)

  //"Track 0"                   // ticks = 0, bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 1"                   // ticks = 2048, bytes = 6
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0xFD, 31, 2,                  // REPEAT: count = 32 - track = 2 (32 * 64 ticks) 
  0xFE,                         // RETURN

  //"Track 2"                   // ticks = 64, bytes = 9
  0xFD, 0, 3,                   // REPEAT: count = 1 - track = 3 ( 6 ticks)
  0x9F + 10,                    // DELAY: ticks = 10             (10 ticks)
  0xFD, 0, 3,                   // REPEAT: count = 1 - track = 3 ( 6 ticks)
  0x9F + 42,                    // DELAY: ticks = 42             (42 ticks)
  0xFE,                         // RETURN

  //"Track 3"                   // ticks = 6, bytes = 9
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0x00 + 1,                     // NOTE ON: note = 1 (delay 1 tick)
  0x41, -16,                    // FX: VOLUME SLIDE ON: steps = -65
  0x9F + 4,                     // DELAY: ticks = 4
  0x43,                         // FX: VOLUME SLIDE OFF
  0x00,                         // NOTE OFF (delay 1 tick)
  0xFE,                         // RETURN

  //"Track 4"                   // ticks = 32 * 64 = 2048, bytes = 6
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFD, 31, 5,                  // REPEAT: count = 32 - track = 5 (64 ticks) 
  0xFE,                         // RETURN

  //"Track 5"                   // ticks = 64, bytes = 7
  0xFD, 5, 7,                   // REPEAT: count = 6 - track = 7  (48 ticks)
  0xFD, 0, 6,                   // REPEAT: count = 1 - track = 6  (16 ticks) 
  0xFE,                         // RETURN

  //"Track 6"                   // ticks = 16, bytes = 7
  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x41, -2,                     // FX: VOLUME SLIDE ON: steps = -2
  0x9F + 16,                    // DELAY: ticks = 16
  0x43,                         // FX: VOLUME SLIDE OFF
  0xFE,                         // RETURN

  //"Track 7"                   // ticks = 8, bytes = 8
  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x41, -16,                    // FX: VOLUME SLIDE ON: steps = -16
  0x9F + 2,                     // DELAY: ticks = 2
  0x43,                         // FX: VOLUME SLIDE OFF
  0x9F + 6,                     // DELAY: ticks = 6
  0xFE,                         // RETURN
};

#endif
