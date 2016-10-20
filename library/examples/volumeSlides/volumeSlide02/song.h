#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x03,          // Number of tracks
  0x00, 0x00,    // Address of track 0
  0x03, 0x00,    // Address of track 1
  0x07, 0x00,    // Address of track 2
  0x00,          // Channel 0 entry track (PULSE)
  0x01,          // Channel 1 entry track (SQUARE)
  0x00,          // Channel 2 entry track (TRIANGLE)
  0x00,          // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,       // FX: SET VOLUME: volume = 0
  0xFE,          // RETURN (empty track used for silent channels)

  //"Track 1"
  0xFD, 32, 2,   // REPEAT: count = 32 - track = 2
  0xFE,          // RETURN

  //"Track 2"
  0x00 +  40,    // NOTE ON: note = 40
  0x40, 0,       // FX: SET VOLUME: volume = 0
  0x41, 4,       // FX: VOLUME SLIDE ON: steps = 4
  0x9F + 32,     // DELAY: ticks = 32
  0x00 +  36,    // NOTE ON: note = 36
  0x41, -4,      // FX: VOLUME SLIDE ON: steps = -4
  0x9F +  32,    // DELAY: ticks = 32
  0xFE,          // RETURN
};

#endif
