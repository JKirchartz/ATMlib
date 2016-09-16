#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song testmusic[] = {   // A) Sample music =>
  0x09,       // Number of tracks
  0x00, 0x00, // Address of track 0
  0x01, 0x00, // Address of track 1
  0x0A, 0x00, // Address of track 2
  0x0E, 0x00, // Address of track 3
  0x1F, 0x00, // Address of track 4
  0x25, 0x00, // Address of track 5
  0x29, 0x00, // Address of track 6
  0x79, 0x00, // Address of track 7
  0x7D, 0x00, // Address of track 8
  0x00,       // Channel 0 entry track (PULSE)
  0x00,       // Channel 1 entry track (SQUARE)
  0x07,       // Channel 2 entry track (TRIANGLE)
  0x05,       // Channel 3 entry track (NOISE)

  //"Track 0",
  0xFE,          // RETURN (empty track used for silent channels)

  //"Track 1",
  0x00 + 20,     // NOTE ON: note = 20
  0x9F +  5,     // DELAY: ticks = 5
  0x3F,          // NOTE OFF
  0x9F +  5,     // DELAY: ticks = 5
  0x00 + 21,     // NOTE ON: note = 21
  0x9F +  5,     // DELAY: ticks = 5
  0x3F,          // NOTE OFF
  0x9F +  5,     // DELAY: ticks = 5
  0xFE,          // RETURN

  //"Track 2",
  0xFD, 32, 1,   // REPEAT: count = 32, track = 1
  
  0xFE,          // RETURN

  //"Track 3",
  0x00 +  8,     // NOTE ON: note = 20
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0x00 +  8,     // NOTE ON: note = 20
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0x00 +  9,     // NOTE ON: note = 21
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0x00 +  9,     // NOTE ON: note = 21
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  
  0xFE,          // RETURN

  //"Track 4",
  0x40, 48,      // FX: SET VOLUME: volume = 48
  0xFD, 32, 3,   // REPEAT: count = 32, track = 3
  
  0xFE,          // RETURN

  //"Track 5",
  0xFD, 32, 6,   // REPEAT: count = 32, track = 6
  0xFE,          // RETURN

  //"Track 6",
  0x40, 32,      // FX: SET VOLUME: volume = 32
  0x9F + 1,      // DELAY: ticks = 1
  0x40, 17,      // FX: SET VOLUME: volume = 17
  0x9F + 1,      // DELAY: ticks = 1
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 14,      // DELAY: ticks = 14

  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 47,      // FX: SET VOLUME: volume = 47
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 15,      // FX: SET VOLUME: volume = 15
  0x9F + 2,      // DELAY: ticks = 2
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 8,      // DELAY: ticks = 8

  0x40, 32,      // FX: SET VOLUME: volume = 32
  0x9F + 1,      // DELAY: ticks = 1
  0x40, 17,      // FX: SET VOLUME: volume = 17
  0x9F + 1,      // DELAY: ticks = 1
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 6,      // DELAY: ticks = 14

  0x40, 32,      // FX: SET VOLUME: volume = 32
  0x9F + 1,      // DELAY: ticks = 1
  0x40, 17,      // FX: SET VOLUME: volume = 17
  0x9F + 1,      // DELAY: ticks = 1
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 6,      // DELAY: ticks = 14

  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 47,      // FX: SET VOLUME: volume = 47
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 15,      // FX: SET VOLUME: volume = 15
  0x9F + 2,      // DELAY: ticks = 2
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 8,      // DELAY: ticks = 8

  0xFE,          // RETURN

  //"Track 7",
  0xFD, 32, 8,    // REPEAT: count = 32, track = 8
  
  0xFE,          // RETURN

  //"Track 8",
  0x00 +  0,     // NOTE ON: note = 0
  0x9F +  4,     // DELAY: ticks = 4
  0x3F,          // NOTE OFF
  0x9F + 26,     // DELAY: ticks = 26

  0x00 +  0,     // NOTE ON: note = 0
  0x9F +  4,     // DELAY: ticks = 4
  0x3F,          // NOTE OFF
  0x9F +  2,     // DELAY: ticks = 2

  0x00 +  0,     // NOTE ON: note = 0
  0x9F +  4,     // DELAY: ticks = 4
  0x3F,          // NOTE OFF
  0x9F + 18,     // DELAY: ticks = 18

  0xFE,          // RETURN
};

#endif
