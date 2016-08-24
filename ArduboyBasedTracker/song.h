#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song testmusic[] = {   // A) Sample music =>
  0x07,       // Number of tracks
  0x00, 0x00, // Address of track 0
  0x01, 0x00, // Address of track 1
  0x0A, 0x00, // Address of track 2
  0x0E, 0x00, // Address of track 3
  0x1F, 0x00, // Address of track 4
  0x25, 0x00, // Address of track 5
  0x50, 0x00, // Address of track 6
  0x02,       // Channel 0 entry track (PULSE)
  0x04,       // Channel 1 entry track (SQUARE)
  0x00,       // Channel 2 entry track (TRIANGLE)
  0x06,       // Channel 3 entry track (NOISE)

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
  0xFD, 31, 1,   // REPEAT: track = 1, count = 32
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
  0xFD, 31, 3,   // REPEAT: track = 3, count = 32
  0xFE,          // RETURN

  //"Track 5",
  //0xE0, 15,      // LONG DELAY: ticks = 80
  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 47,      // FX: SET VOLUME: volume = 47
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 2,      // DELAY: ticks = 2
  0x40, 15,      // FX: SET VOLUME: volume = 15
  0x9F + 2,      // DELAY: ticks = 2
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 4,      // DELAY: ticks = 4
  /*
    0x40, 63,      // FX: SET VOLUME: volume = 63
    0x9F + 1,      // DELAY: ticks = 4
    0x40, 47,      // FX: SET VOLUME: volume = 47
    0x9F + 1,      // DELAY: ticks = 4
    0x40, 31,      // FX: SET VOLUME: volume = 31
    0x9F + 1,      // DELAY: ticks = 4
    0x40, 15,      // FX: SET VOLUME: volume = 15
    0x9F + 1,      // DELAY: ticks = 4
    0x40,  0,      // FX: SET VOLUME: volume = 0
    0x9F + 8,      // DELAY: ticks = 8
  */

  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 1,      // DELAY: ticks = 1
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 1,      // DELAY: ticks = 1
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 2,      // DELAY: ticks = 2

  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 1,      // DELAY: ticks = 1
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 1,      // DELAY: ticks = 1
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 1,      // DELAY: ticks = 2

  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 1,      // DELAY: ticks = 1
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 1,      // DELAY: ticks = 1
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0x9F + 3,      // DELAY: ticks = 2



  0xFE,          // RETURN

  //"Track 6",
  0xFD, 31, 5,    // REPEAT: track = 5, count = 32
  0xFE          // RETURN
};

#endif
