#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {                // total song in bytes = 65
  // setup bytes 11
  0x05,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x0B, 0x00,                   // Address of track 2
  0x0E, 0x00,                   // Address of track 3
  0x11, 0x00,                   // Address of track 4
  0x00,                         // Channel 0 entry track (PULSE)
  0x01,                         // Channel 1 entry track (SQUARE)
  0x02,                         // Channel 2 entry track (TRIANGLE)
  0x03,                         // Channel 3 entry track (NOISE)

  //"Track 0"                   // ticks = 0 / bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 1"                   // ticks = 0 / bytes = 8
  0x9D, 16,                     // SET song tempo: value = 16
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0xFD, 1,  4,                  // REPEAT: count = 2 / track = 8 (2 *32 ticks)
  0x9F,                         // STOP channel

  //"Track 2"                   // ticks = 0 / bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 3"                   // ticks = 0 / bytes = 3
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 4"                   // ticks = 32 / bytes = 33
  36,
  160,
  0,
  160,
  31,
  160,
  0,
  160,
  27,
  160,
  0,
  162,
  36,
  160,
  0,
  162,
  31,
  160,
  0,
  162,
  27,
  160,
  0,
  160,
  31,
  160,
  0,
  162,
  24,
  160,
  0,
  168,
  254,
};

#endif
