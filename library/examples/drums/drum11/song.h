#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x04,           // Number of tracks
  0x00, 0x00,     // Address of track 0
  0x03, 0x00,     // Address of track 1
  0x07, 0x00,     // Address of track 2
  0x19, 0x00,     // Address of track 3
  0x00,           // Channel 0 entry track (PULSE)
  0x00,           // Channel 1 entry track (SQUARE)
  0x00,           // Channel 2 entry track (TRIANGLE)
  0x01,           // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,        // FX: SET VOLUME: volume = 0
  0xFE,           // RETURN

  //"Track 1"
  0xFD, 32, 2,    // REPEAT: count = 32 - track = 2
  0xFE,           // RETURN

  //"Track 2"
  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x9F + 1,       // DELAY: ticks = 1
  0x40,  0,       // FX: SET VOLUME: volume = 0

  0x9F + 15,      // DELAY: ticks = 15

  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x9F + 1,       // DELAY: ticks = 1
  0x40,  0,       // FX: SET VOLUME: volume = 0

  0x9F + 15,      // DELAY: ticks = 15

  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x41, -4,       // FX: VOLUME SLIDE ON: steps = -4
  0x9F + 16,       // DELAY: ticks = 15
  0x43,           // FX: VOLUME SLIDE OFF
  0x9F + 16,      // DELAY: ticks = 16

  0xFE,           // RETURN

};

#endif
