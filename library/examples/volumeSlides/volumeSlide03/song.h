#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x03,          // Number of tracks
  0x00, 0x00,    // Address of track 0
  0x03, 0x00,    // Address of track 1
  0x0A, 0x00,    // Address of track 2
  0x00,          // Channel 0 entry track (PULSE)
  0x01,          // Channel 1 entry track (SQUARE)
  0x00,          // Channel 2 entry track (TRIANGLE)
  0x00,          // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,       // FX: SET VOLUME: volume = 0
  0xFE,          // RETURN (empty track used for silent channels)

  //"Track 1"
  0x40, 0,       // FX: SET VOLUME: volume = 63
  0xFD, 15, 2,   // REPEAT: count = 15 - track = 2
  0x3F,          // NOTE OFF
  0xFE,          // RETURN

  //"Track 2"
  0x41, 1,       // FX: VOLUME SLIDE ON: steps = 4
  0x00 +  36,    // NOTE ON: note = 36
  0x9F +  4,     // DELAY: ticks = 4
  0x3F,          // NOTE OFF
  0x9F +  4,     // DELAY: ticks = 4
  0x00 +  40,    // NOTE ON: note = 36
  0x9F +  4,     // DELAY: ticks = 4
  0x3F,          // NOTE OFF
  0x9F +  4,     // DELAY: ticks = 4
  0x00 +  43,    // NOTE ON: note = 36
  0x9F +  8,     // DELAY: ticks = 4
  0x3F,          // NOTE OFF
  0x9F +  8,     // DELAY: ticks = 4
  0xFE,          // RETURN
};

#endif
