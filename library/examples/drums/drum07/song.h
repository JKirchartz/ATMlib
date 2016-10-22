#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x06,          // Number of tracks
  0x00, 0x00,    // Address of track 0
  0x03, 0x00,    // Address of track 1
  0x09, 0x00,    // Address of track 2
  0x12, 0x00,    // Address of track 3
  0x19, 0x00,    // Address of track 4
  0x1D, 0x00,    // Address of track 5
  0x01,          // Channel 0 entry track (PULSE)
  0x00,          // Channel 1 entry track (SQUARE)
  0x00,          // Channel 2 entry track (TRIANGLE)
  0x04,          // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,       // FX: SET VOLUME: volume = 0
  0xFE,          // RETURN (empty track used for silent channels)

  //"Track 1"
  0x40, 63,      // FX: SET VOLUME: volume = 63
  0xFD, 31, 2,   // REPEAT: count = 32 - track = 2
  0xFE,          // RETURN

  //"Track 2"
  0xFD, 0, 3,     // REPEAT: count = 1 - track = 3
  0x9F + 10,      // DELAY: ticks = 12
  0xFD, 0, 3,     // REPEAT: count = 1 - track = 3
  0x9F + 42,      // DELAY: ticks = 44
  0xFE,           // RETURN

  //"Track 3"
  0x00 + 1,      // NOTE ON: note = 1
  0x44, -65,     // FX: VOLUME SLIDE ON: steps = -65
  0x9F + 4,      // DELAY: ticks = 4
  0x46,          // FX: VOLUME SLIDE OFF
  0x00 ,         // NOTE OFF
  0xFE,          // RETURN

  //"Track 4"
  0xFD, 31, 5,   // REPEAT: count = 32 - track = 5
  0xFE,          // RETURN

  //"Track 5"
  0x9F + 32,      // DELAY: ticks = 32
  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x41, -2,       // FX: VOLUME SLIDE ON: steps = -2
  0x9F + 16,      // DELAY: ticks = 16
  0x43,           // FX: VOLUME SLIDE OFF
  0x9F + 16,      // DELAY: ticks = 16
  0xFE,           // RETURN
};

#endif
