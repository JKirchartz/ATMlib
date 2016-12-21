#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {                // total song in bytes = 61
  //                            // setup bytes 15
  0x05,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x11, 0x00,                   // Address of track 2
  0x20, 0x00,                   // Address of track 3
  0x27, 0x00,                   // Address of track 4
  0x00,                         // Channel 0 entry track (PULSE)
  0x00,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x01,                         // Channel 3 entry track (NOISE)

  //"Track 0"                   // ticks = 0 / bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 1"                   // ticks = 160 / bytes = 14
  0x9D, 50,                     // SET song tempo: value = 50
  0xFD, 0, 2,                   // REPEAT: count = 1 / track = 2 (48 ticks)
  0x9F + 8,                     // DELAY: ticks = 8              ( 8 ticks)
  0xFD, 0, 2,                   // REPEAT: count = 1 / track = 2 (48 ticks)
  0x9F + 8,                     // DELAY: ticks = 8              ( 8 ticks)
  0xFD, 0, 2,                   // REPEAT: count = 1 / track = 2 (48 ticks)
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 2"                   // ticks = 48 / bytes = 15
  0xFD,  5, 3,                  // REPEAT: count = 6 / track = 3 (12 ticks)
  0x40,  0,                     // FX: SET VOLUME: volume = 0
  0x9F + 4,                     // DELAY: ticks = 4              ( 4 ticks)
  0xFD, 0, 4,                   // REPEAT: count = 1 / track = 4 ( 4 ticks)
  0x9F + 12,                    // DELAY: ticks = 12             (12 ticks)
  0xFD, 0, 4,                   // REPEAT: count = 1 / track = 4 ( 4 ticks)
  0x9F + 12,                    // DELAY: ticks = 12             (12 ticks)
  0xFE,                         // RETURN

  //"Track 3"                   // ticks = 2 / bytes = 7
  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x41, -8,                     // FX: VOLUME SLIDE ON: steps = -8
  0x9F + 2,                     // DELAY: ticks = 2
  0x43,                         // FX: VOLUME SLIDE OFF
  0xFE,                         // RETURN

  //"Track 4"                   // ticks = 4 / bytes = 7
  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x41, -8,                     // FX: VOLUME SLIDE ON: steps = -8
  0x9F + 4,                     // DELAY: ticks = 4
  0x43,                         // FX: VOLUME SLIDE OFF
  0xFE,                         // RETURN
  
};

#endif
