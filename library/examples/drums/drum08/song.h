#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x08,           // Number of tracks
  0x00, 0x00,     // Address of track 0
  0x03, 0x00,     // Address of track 1
  0x09, 0x00,     // Address of track 2
  0x12, 0x00,     // Address of track 3
  0x19, 0x00,     // Address of track 4
  0x1F, 0x00,     // Address of track 5
  0x27, 0x00,     // Address of track 6
  0x2E, 0x00,     // Address of track 7
  0x01,           // Channel 0 entry track (PULSE)
  0x00,           // Channel 1 entry track (SQUARE)
  0x00,           // Channel 2 entry track (TRIANGLE)
  0x04,           // Channel 3 entry track (NOISE)

  //"Track 0"     // 3
  0x40, 0,        // FX: SET VOLUME: volume = 0
  0xFE,           // RETURN (empty track used for silent channels)

  //"Track 1"     // 6
  0x40, 63,       // FX: SET VOLUME: volume = 63
  0xFD, 31, 2,    // REPEAT: count = 32 - track = 2
  0xFE,           // RETURN

  //"Track 2"     // 9
  0xFD, 0, 3,     // REPEAT: count = 1 - track = 3
  0x9F + 8,       // DELAY: ticks = 8
  0xFD, 0, 3,     // REPEAT: count = 1 - track = 3
  0x9F + 39,      // DELAY: ticks = 39
  0xFE,           // RETURN

  //"Track 3"     // 7   ticks 8
  0x00 + 1,       // NOTE ON: note = 1
  0x44, -65,      // FX: VOLUME SLIDE ON: steps = -65
  0x9F + 3,       // DELAY: ticks = 3
  0x46,           // FX: VOLUME SLIDE OFF
  0x00 ,          // NOTE OFF
  0xFE,           // RETURN

  //"Track 4"     // 6
  0x40, 0,        // FX: SET VOLUME: volume = 0
  0xFD, 31, 5,    // REPEAT: count = 32 - track = 5
  0xFE,           // RETURN

  //"Track 5"     // 8
  0xFD, 2, 7,     // REPEAT: count = 4 - track = 7
  0xFD, 0, 6,     // REPEAT: count = 1 - track = 6 
  0x9F + 16,      // DELAY: ticks = 16
  0xFE,           // RETURN

  //"Track 6"     // 7 ticks 19
  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x41, -2,       // FX: VOLUME SLIDE ON: steps = -2
  0x9F + 16,      // DELAY: ticks = 16
  0x43,           // FX: VOLUME SLIDE OFF
  0xFE,           // RETURN

  //"Track 7"     // 8 ticks 8
  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x41, -16,      // FX: VOLUME SLIDE ON: steps = -16
  0x9F + 2,       // DELAY: ticks = 2
  0x43,           // FX: VOLUME SLIDE OFF
  0x9F + 6,       // DELAY: ticks = 2
  0xFE,           // RETURN
};

#endif
